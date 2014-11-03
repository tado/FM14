#include "ofApp.h"

#include "StSimplePixelate.h"
#include "StCvOpDraw.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(30);
    blackmagic = new BlackmagicCapture(1920, 1080, 60.0);

    // StateMachine
    stateMachine.addState<StSimplePixelate>();
    stateMachine.addState<StCvOpDraw>();
    stateMachine.changeState("StSimplePixelate");
    guiVisible = false;
    
    // GUI
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("MAIN");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
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
    switch (key) {
            
        case '1':
            stateMachine.changeState("StSimplePixelate");
            break;
        case '2':
            stateMachine.changeState("StCvOpDraw");
            break;
       
            //---------------------------------------------------

        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            gui->toggleVisible();
            stateMachine.getSharedData().guiVisible?
            ofHideCursor() : ofShowCursor();
            stateMachine.getSharedData().guiVisible?
            stateMachine.getSharedData().guiVisible = false :stateMachine.getSharedData().guiVisible = true;
            break;
            
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
