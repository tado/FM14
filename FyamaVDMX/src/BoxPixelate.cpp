#include "BoxPixelate.h"
#include "testApp.h"

void BoxPixelate::setup(){
    int camWidth = 1920;
    int camHeight = 351;
    radius = 10;
    num = camWidth * camHeight / radius;
    
    for (int i = 0; i<num; i++) {
        float a = ofRandom(360);
        float l = ofRandom(10);
        angle.push_back(a);
        length.push_back(l);
    }
}

void BoxPixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void BoxPixelate::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofBackground(0);
    float width = pixels.getWidth();
    float height = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(width);
    ratio.y = ofGetHeight() / float(height);
    
    ofPushMatrix();
    ofTranslate(radius / 2.0 * ratio.x, radius / 2.0 * ratio.y);
    float scale = 0.01;
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0 && angle.size() > 0 && length.size() > 0){
        for (int i = 0; i < width; i+=radius){
            for (int j = 0; j < height; j+=radius){
                unsigned char r = pixels[(j * width + i)*3];
                unsigned char g = pixels[(j * width + i)*3+1];
                unsigned char b = pixels[(j * width + i)*3+2];
                
                ofColor col = ofColor(r, g, b);
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, sat * 1.2, 180);
                ofSetColor(col, 127);
                
                ofPushMatrix();
                ofTranslate(i * ratio.x, j * ratio.y);
                
                int n = j * width / radius + i / radius;
                
                float curAngle = angle[n] + ((r + g + b) * 2.0 - angle[n]) / 60.0;
                ofRotateX(curAngle);
                ofRotateY(curAngle * 1.2);
                ofRotateZ(curAngle * 0.2);
                angle[n] = curAngle;
                
                float curLength = length[n] + ((r + g + b) * 1.0 - length[n]) / 20.0;
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofRect(0, 0, radius * 8, radius * 8 * br / 255);
                ofSetRectMode(OF_RECTMODE_CORNER);
                length[n] = curLength;
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string BoxPixelate::getName(){
    return "box";
}