#include "NofillPixelate.h"
#include "testApp.h"

void NofillPixelate::setup(){
    radius = 20;
}

void NofillPixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void NofillPixelate::draw(){
    ofVec2f scale;
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    scale.x = SCREEN_WIDTH / float(camWidth);
    scale.y = SCREEN_HEIGHT / float(camHeight);
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofBackground(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofPushMatrix();
    ofTranslate(0, radius / 2.0);
    
    ofNoFill();
    ofSetLineWidth(4.0);
    
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i) * 3];
                unsigned char g = pixels[(j * camWidth + i) * 3 + 1];
                unsigned char b = pixels[(j * camWidth + i) * 3 + 2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)r / 50.0);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)g / 50.0);
                ofSetColor(0, 0, 255, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)b / 50.0);
                
            }
        }
        ofDisableBlendMode();
    }
    ofSetLineWidth(1.0);
    ofPopMatrix();
    ofFill();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string NofillPixelate::getName(){
    return "nofill";
}