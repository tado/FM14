#include "NofillPixelate.h"
#include "ofApp.h"

void NofillPixelate::setup(){
    gui.setup();
    gui.add(radius.setup("Nofill Radius", 20, 1, 50));
    gui.add(srcLevel.setup("Nofill Level", 0, 0, 255));
    gui.add(circleScale.setup("Nofill scale", 0.02, 0.0, 0.1));
    gui.loadFromFile("settings.xml");
}

void NofillPixelate::update(){
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void NofillPixelate::draw(){
    ofVec2f scale;
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    scale.x = SCREEN_WIDTH / float(camWidth);
    scale.y = SCREEN_HEIGHT / float(camHeight);
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(srcLevel);
    tex.loadData(((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
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
                ofCircle(i * scale.x, j * scale.y, radius * (float)r  * circleScale);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)g  * circleScale);
                ofSetColor(0, 0, 255, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)b  * circleScale);
                
            }
        }
    }
    ofDisableBlendMode();
    ofSetLineWidth(1.0);
    ofPopMatrix();
    ofFill();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string NofillPixelate::getName(){
    return "nofill";
}