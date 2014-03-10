#include "LinePixelate.h"
#include "testApp.h"

void LinePixelate::setup(){
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    int camWidth = 640;
    int camHeight = 117;
    radius = 4;
    num = camWidth * camHeight / radius;

    for (int i = 0; i<num; i++) {
        float a = ofRandom(360);
        float l = ofRandom(10);
        angle.push_back(a);
        length.push_back(l);
    }
}

void LinePixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void LinePixelate::draw(){
    ofBackground(0);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    ofTranslate(radius / 2.0 * ratio.x, radius / 2.0 * ratio.y);
    float scale = 0.01;
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if (pixels.size()>0 && angle.size() > 0 && length.size() > 0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofPushMatrix();
                ofTranslate(i * ratio.x, j * ratio.y);
                
                int n = j * camWidth / radius + i / radius;
                
                float curAngle = angle[n] + ((r + g + b) * 2.0 - angle[n]) / 80.0;
                ofRotateZ(curAngle);
                angle[n] = curAngle;
                
                float curLength = length[n] + ((r + g + b) * radius * ratio.y / 1000.0 - length[n]) / 20.0;
                ofSetColor(r, g, b);
                ofRect(0, 0, curLength, curLength * 6.0);
                //ofEllipse(0, 0, curLength, curLength * 12.0);
                length[n] = curLength;
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string LinePixelate::getName(){
    return "line";
}