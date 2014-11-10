#include "ofApp.h"
#include "StBlank.h"
#include "StFftSimplePlot.h"
#include "StFftDrawBar.h"
#include "StFftDrawCircle.h"
#include "StSoundWave.h"
#include "StFftBox.h"
#include "StFftTracer.h"
#include "StFftRibbon.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    // FFT
    bufferSize = 1024;
    fft = new FFTData(bufferSize);
    
    // FBO
    drawFbo = new DrawFbo();
    
    stateMachine.addState<StBlank>();
    stateMachine.addState<StSoundWave>();
    stateMachine.addState<StFftSimplePlot>();
    stateMachine.addState<StFftDrawBar>();
    stateMachine.addState<StFftDrawCircle>();
    stateMachine.addState<StFftBox>();
    stateMachine.addState<StFftTracer>();
    stateMachine.addState<StFftRibbon>();
    stateMachine.changeState("StBlank");
        
    // GUI
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("MAIN");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    gui->addSlider("FFT SCALE", 0.0, 20.0, 5.0);
    gui->addSlider("FFT POW", 0.0, 1.0, 0.5);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxUISlider *gfftscale = (ofxUISlider *)gui->getWidget("FFT SCALE"); float fftscale = gfftscale->getValue();
    ofxUISlider *gfftpow = (ofxUISlider *)gui->getWidget("FFT POW"); float fftpow = gfftpow->getValue();
    fft->scale = fftscale;
    fft->pow = fftpow;
    fft->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawFbo->draw();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
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
            stateMachine.changeState("StSoundWave");
            break;
        case '3':
            stateMachine.changeState("StFftDrawBar");
            break;
        case '4':
            stateMachine.changeState("StFftDrawCircle");
            break;
        case '5':
            stateMachine.changeState("StFftBox");
            break;
        case '6':
            stateMachine.changeState("StFftRibbon");
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
