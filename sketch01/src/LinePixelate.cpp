#include "LinePixelate.h"

void LinePixelate::setup(){
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    radius = 3;
    num = 100000000;
    for (int i = 0; i<num; i++) {
        float a = ofRandom(360);
        float l = ofRandom(10);
        angle.push_back(a);
        length.push_back(l);
    }
}

void LinePixelate::update(){
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
	}
}

void LinePixelate::draw(){
    ofBackground(0);
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    float radius = 5;
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
                
                float curAngle = angle[(j * camWidth + i) * 3] + ((r + g + b) * 2.0 - angle[(j * camWidth + i) * 3]) / 80.0;
                ofRotateZ(curAngle);
                angle[(j * camWidth + i)*3] = curAngle;
                
                float curLength = length[(j * camWidth + i)*3] + ((r + g + b) * radius * ratio.y / 400.0 - length[(j * camWidth + i)*3]) / 20.0;
                ofSetColor(r, g, b, 100);
                //ofRect(0, 0, curLength, curLength * 8.0);
                ofEllipse(0, 0, curLength, curLength * 12.0);
                length[(j * camWidth + i) * 3] = curLength;
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
}

string LinePixelate::getName(){
    return "line";
}