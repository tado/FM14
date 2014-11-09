#include "StBlank.h"

string StBlank::getName(){
    return "StBlank";
}

void StBlank::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
}

void StBlank::update(){
    app = ((ofApp*)ofGetAppPtr());
}

void StBlank::draw(){
    gui->setVisible(getSharedData().guiVisible);
    ofSetColor(255);
    ofDrawBitmapString("Max Value = " + ofToString(app->fft->maxValue), 10, 10);
}

void StBlank::guiEvent(ofxUIEventArgs &e){

}

void StBlank::stateExit(){
    gui->setVisible(false);
}
