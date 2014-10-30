#include "StopMotion.h"
#include "ofApp.h"

void StopMotion::setup(){
    getSharedData().stop = 0;
    lastState = false;
}

void StopMotion::update(){
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void StopMotion::draw(){
    ofDisableBlendMode();
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    if (getSharedData().stop == 0) {
        tex.loadData(pixels);
        drawTex = tex;
    }
    ofSetColor(255);
    drawTex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    ofDrawBitmapString("Stop = " + ofToString(getSharedData().stop, 0), 20, 20);
}

string StopMotion::getName(){
    return "stopmotion";
}
