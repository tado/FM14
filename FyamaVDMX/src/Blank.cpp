#include "Blank.h"
#include "ofApp.h"

void Blank::setup(){
}

void Blank::update(){
    // pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void Blank::draw(){
    ofDisableBlendMode();

    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishScreen();

    /*
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofSetColor(255);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
     */
}

string Blank::getName(){
    return "blank";
}
