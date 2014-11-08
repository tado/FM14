#include "ofApp.h"
#include "StBlank.h"
#include "StFftSimplePlot.h"
#include "StFftDrawBar.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    stateMachine.addState<StBlank>();
    stateMachine.addState<StFftSimplePlot>();
    stateMachine.addState<StFftDrawBar>();
    stateMachine.changeState("StBlank");
    
    // SoundStream
    int bufferSize = 512;
    ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
    
    // FFT
    fft = new FFTData(bufferSize);
    
    // GUI
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("MAIN");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxUICanvas *gui;
}

//--------------------------------------------------------------
void ofApp::draw(){

}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    fft->audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
            
        case '1':
            stateMachine.changeState("StBlank");
            break;
        case '2':
            stateMachine.changeState("StFftSimplePlot");
            break;
        case '3':
            stateMachine.changeState("StFftDrawBar");
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
