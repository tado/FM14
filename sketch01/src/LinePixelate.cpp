#include "LinePixelate.h"

void LinePixelate::setup(){
    ofSetRectMode(OF_RECTMODE_CENTER);
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
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofPushMatrix();
                ofTranslate(i * ratio.x, j * ratio.y);
                ofRotateZ((r+g+b) / 1.2);
                ofSetColor(r, g, b, 200);
                ofRect(0, 0, radius*ratio.x / 2.0, (r + g + b) * radius * ratio.y / 200.0);
                ofPopMatrix();
                
            }
        }
    }
    ofDisableBlendMode();
}

string LinePixelate::getName(){
    return "line";
}