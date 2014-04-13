#include "StopMotion.h"
#include "testApp.h"

void StopMotion::setup(){
    getSharedData().stop = 0;
    lastState = false;
}

void StopMotion::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void StopMotion::draw(){
    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    if (getSharedData().stop == 0) {
        tex.loadData(pixels);
        drawTex = tex;
    }
    ofSetColor(255);
    drawTex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    ofDrawBitmapString("Stop = " + ofToString(getSharedData().stop, 0), 20, 20);
}

string StopMotion::getName(){
    return "stopmotion";
}
