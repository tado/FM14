#include "Through.h"
#include "ofApp.h"

void Through::setup(){
}

void Through::update(){
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void Through::draw(){
    ofDisableBlendMode();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofSetColor(255);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
}

string Through::getName(){
    return "through";
}
