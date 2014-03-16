#include "Blank.h"
#include "testApp.h"

void Blank::setup(){
}

void Blank::update(){
}

void Blank::draw(){
    ofDisableBlendMode();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string Blank::getName(){
    return "blank";
}
