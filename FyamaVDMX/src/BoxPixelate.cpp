#include "BoxPixelate.h"
#include "testApp.h"

void BoxPixelate::setup(){
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    int camWidth = 640;
    int camHeight = 117;
    radius = 5;
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
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    ofTranslate(radius / 2.0 * ratio.x, radius / 2.0 * ratio.y);
    float scale = 0.01;
    
    //ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0 && angle.size() > 0 && length.size() > 0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofColor col = ofColor(r, g, b);
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, sat * 1.2, 180);
                ofSetColor(col, 127);
                
                ofPushMatrix();
                ofTranslate(i * ratio.x, j * ratio.y);
                
                int n = j * camWidth / radius + i / radius;
                
                float curAngle = angle[n] + ((r + g + b) * 2.0 - angle[n]) / 60.0;
                ofRotateX(curAngle);
                ofRotateY(curAngle * 1.2);
                ofRotateZ(curAngle * 0.2);
                angle[n] = curAngle;
                
                float curLength = length[n] + ((r + g + b) * radius * ratio.y / 1000.0 - length[n]) / 20.0;
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofRect(0, 0, radius * 14, radius * 14 * br / 255);
                //ofEllipse(0, 0, curLength, curLength * 12.0);
                ofSetRectMode(OF_RECTMODE_CORNER);
                length[n] = curLength;
                ofPopMatrix();
            }
        }
    }
    //ofDisableDepthTest();
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string BoxPixelate::getName(){
    return "box";
}