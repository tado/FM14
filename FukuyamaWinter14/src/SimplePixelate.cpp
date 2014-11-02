#include "SimplePixelate.h"
#include "ofApp.h"

void SimplePixelate::setup(){
}

void SimplePixelate::update(){
    pixels = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
}

void SimplePixelate::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = 1920;
    int camHeight = 1080;
    float radius = 50.0;
    float hue = 1.0;
    float sat = 1.5;
    float br = 0.5;
    float circleScale = 1.5;
    
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                ofColor col = pixels.getColor(i + radius / 2.0, j + radius / 2.0);
                int h = col.getHue();
                int s = col.getSaturation();
                int v = col.getBrightness();
                col.setHsb(h * hue, s * sat, v * br);
                
                float r = col.r / 255.0;
                float g = col.g / 255.0;
                float b = col.b / 255.0;
                
                ofPushMatrix();
                ofTranslate(radius/2.0, radius/2.0);
                ofSetColor(255, 0, 0, br * 255);
                ofCircle(i + radius * 0.2, j, radius * (float)r * circleScale);
                ofSetColor(0, 255, 0, br * 255);
                ofCircle(i, j, radius * (float)g * circleScale);
                ofSetColor(0, 0, 255, br * 255);
                ofCircle(i - radius * 0.2, j, radius * (float)g * circleScale);
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();

}

string SimplePixelate::getName(){
    return "simple";
}
