#include "ofApp.h"
#include "StBlank.h"
#include "StFftSimplePlot.h"
#include "StFftDrawBar.h"
#include "StFftDot.h"
#include "StFftDrawCircle.h"
#include "StSoundWave.h"
#include "StFftBox.h"
#include "StFftRibbon.h"
#include "StFftRings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    // FFT
    bufferSize = 1024;
    fft = new FFTData(bufferSize);

    // Blackmagic Capture
    blackmagic = new BlackmagicCapture(1920, 1080, 60.0);
    
    // FBO
    drawFbo = new DrawFbo();
    
    stateMachine.addState<StBlank>();
    stateMachine.addState<StSoundWave>();
    stateMachine.addState<StFftSimplePlot>();
    stateMachine.addState<StFftDrawBar>();
    stateMachine.addState<StFftDot>();
    stateMachine.addState<StFftDrawCircle>();
    stateMachine.addState<StFftBox>();
    stateMachine.addState<StFftRibbon>();
    stateMachine.addState<StFftRings>();
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
    gui->addIntSlider("MIX", 0, 255, 255);
    vector<string> names;
    names.push_back("BLACKMAGIC"); names.push_back("INTERNAL CAM"); names.push_back("MOVIE");
    ofxUIDropDownList *ddl;
    ddl = gui->addDropDownList("INPUT SOURCE", names);
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(true);
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

    ofxUISlider *gfftscale = (ofxUISlider *)gui->getWidget("FFT SCALE"); float fftscale = gfftscale->getValue();
    ofxUISlider *gfftpow = (ofxUISlider *)gui->getWidget("FFT POW"); float fftpow = gfftpow->getValue();
    fft->scale = fftscale;
    fft->pow = fftpow;
    fft->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofxUIIntSlider *gmix = (ofxUIIntSlider *)gui->getWidget("MIX"); int mix = gmix->getValue();
    ofSetColor(mix);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    blackmagic->drawSub();
    
    ofSetColor(255);
    drawFbo->draw();
    ofDisableAlphaBlending();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
    if (name == "INPUT SOURCE") {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        vector<int> &selectID = ddlist->getSelectedIndeces();
        for (int i = 0; i < selectID.size(); i++) {
            blackmagic->changeInput(selectID[i]);
            cout << "Selected Video ID : " << selectID[i] << endl;
        }
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
            stateMachine.changeState("StFftDot");
            break;
        case '5':
            stateMachine.changeState("StFftDrawCircle");
            break;
        case '6':
            stateMachine.changeState("StFftBox");
            break;
        case '7':
            stateMachine.changeState("StFftRibbon");
            break;
        case '8':
            stateMachine.changeState("StFftRings");
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
