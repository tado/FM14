#include "SimplePixelate.h"
#include "testApp.h"

void SimplePixelate::setup(){
    radius = 6;
    int width = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int height = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(width, height, 3);
}

void SimplePixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void SimplePixelate::draw(){
    ofVec2f scale;
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    scale.x = ofGetWidth() / float(camWidth);
    scale.y = ofGetHeight() / float(camHeight);
    
    ofBackground(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i) * 3];
                unsigned char g = pixels[(j * camWidth + i) * 3 + 1];
                unsigned char b = pixels[(j * camWidth + i) * 3 + 2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle(i * scale.x + radius * 1.2, j * scale.y, radius * (float)r / 100.0);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)g / 100.0);
                ofSetColor(0, 0, 255, 180);
                ofCircle(i * scale.x -  radius * 1.2, j * scale.y, radius * (float)b / 100.0);
                
            }
        }
        ofDisableBlendMode();
    }
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.gui.draw();
}

string SimplePixelate::getName(){
    return "simple";
}