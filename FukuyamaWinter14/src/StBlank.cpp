#include "StBlank.h"
#include "ofApp.h"

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
    ofAddListener(gui->newGUIEvent,this,&StBlank::guiEvent);
}

void StBlank::update(){

}

void StBlank::draw(){
    gui->setVisible(getSharedData().guiVisible);
}

void StBlank::guiEvent(ofxUIEventArgs &e){

}

void StBlank::stateExit(){
    gui->setVisible(false);
}
