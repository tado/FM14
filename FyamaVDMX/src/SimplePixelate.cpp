#include "SimplePixelate.h"
#include "testApp.h"

void SimplePixelate::setup(){
    radius = 4;
}

void SimplePixelate::update(){
    ((testApp*)ofGetAppPtr())->syphonIO.texture.readToPixels(pixels);
}

void SimplePixelate::draw(){
    ofVec2f scale;
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    scale.x = ofGetWidth() / float(camWidth);
    scale.y = ofGetHeight() / float(camHeight);
    
    ofBackground(0);
    ofPushMatrix();
    ofTranslate(radius / 2.0 * scale.x, radius / 2.0 * scale.y);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle((i - radius / 2.0) * scale.x, j * scale.y, radius*(float)r/255.0 * scale.x * 0.7);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius*(float)g/255.0 * scale.x * 0.7);
                ofSetColor(0, 0, 255, 180);
                ofCircle((i + radius / 2.0) * scale.x, j * scale.y, radius*(float)b/255.0 * scale.x * 0.7);
                
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string SimplePixelate::getName(){
    return "simple";
}