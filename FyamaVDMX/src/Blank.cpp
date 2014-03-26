#include "Blank.h"
#include "testApp.h"

void Blank::setup(){
}

void Blank::update(){
    // pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void Blank::draw(){
    ofDisableBlendMode();

    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();

    /*
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofSetColor(255);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
     */
}

string Blank::getName(){
    return "blank";
}
