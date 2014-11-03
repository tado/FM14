#include "ofApp.h"

#include "SimplePixelate.h"
#include "DrawOpticalFlow.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    blackmagic = new BlackmagicCapture(1920, 1080, 60.0);

    // StateMachine
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<DrawOpticalFlow>();
    stateMachine.changeState("DrawOpticalFlow");
    guiVisible = false;
    
    // GUI
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("MAIN");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    //gui->addToggle("FULLSCREEN", false);
    gui->addIntSlider("MIX", 0, 255, 255);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    blackmagic->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofxUIIntSlider *m = (ofxUIIntSlider *)gui->getWidget("MIX");
    int mix = m->getValue();
    ofSetColor(mix);
    blackmagic->draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    blackmagic->exit();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
    if(name == "FULLSCREEN"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'g') {
        gui->toggleVisible();
        guiVisible? ofHideCursor() : ofShowCursor();
        guiVisible? guiVisible = false : guiVisible = true;
    }
    if (key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
