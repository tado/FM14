#include "ofApp.h"

#include "StBlank.h"
#include "StSimplePixelate.h"
#include "StCvOpDraw.h"
#include "StCvOpParticle.h"
#include "StCvOpParticlePath.h"
#include "StCvOpMesh.h"
#include "StCvOpNote.h"
#include "StCvOpDistort.h"
#include "StSakuraParticle.h"
#include "StSoundWave.h"
#include "StFftDrawCircle.h"
#include "StFftDot.h"
#include "StFftDrawBar.h"
#include "StFftBox.h"
#include "StFftRibbon.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetWindowPosition(1920, 0);
    
    // FFT
    bufferSize = 1024;
    fft = new FFTData(bufferSize);
    
    // Blackmagic
    blackmagic = new BlackmagicCapture(1920, 1080, 60.0);

    // StateMachine
    stateMachine.addState<StBlank>();
    stateMachine.addState<StSimplePixelate>();
    stateMachine.addState<StCvOpDraw>();
    stateMachine.addState<StCvOpParticle>();
    stateMachine.addState<StCvOpParticlePath>();
    stateMachine.addState<StCvOpMesh>();
    stateMachine.addState<StCvOpNote>();
    stateMachine.addState<StCvOpDistort>();
    stateMachine.addState<StSakuraParticle>();
    stateMachine.addState<StSoundWave>();
    stateMachine.addState<StFftDrawCircle>();
    stateMachine.addState<StFftDot>();
    stateMachine.addState<StFftDrawBar>();
    stateMachine.addState<StFftBox>();
    stateMachine.addState<StFftRibbon>();
    stateMachine.changeState("StBlank");
    guiVisible = false;
    
    // FBO
    drawFbo = new DrawFbo();
    
    // OSC Control
    oscControl = new OscControl();
    oscControl->setup();
    srcMix = 255;
    fxMix = 255;
    
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
    gui->addSpacer();
    gui->addIntSlider("MIX", 0, 255, 255);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->addSpacer();
    vector<string> names;
    names.push_back("BLACKMAGIC"); names.push_back("INTERNAL CAM"); names.push_back("MOVIE");
    ofxUIDropDownList *ddl;
    ddl = gui->addDropDownList("INPUT SOURCE", names);
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(true);
    
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    blackmagic->update();
    oscControl->update();
    
    // FFT
    ofxUISlider *gfftscale = (ofxUISlider *)gui->getWidget("FFT SCALE"); float fftscale = gfftscale->getValue();
    ofxUISlider *gfftpow = (ofxUISlider *)gui->getWidget("FFT POW"); float fftpow = gfftpow->getValue();
    fft->scale = fftscale;
    fft->pow = fftpow;
    fft->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofxUIIntSlider *m = (ofxUIIntSlider *)gui->getWidget("MIX");
    
    //srcMix = m->getValue();
    srcMix = oscControl->controlVal[0] * 2;
    
    ofSetColor(srcMix);
    blackmagic->drawSub();

    fxMix = oscControl->controlVal[1] * 2;
    ofSetColor(fxMix);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    drawFbo->draw();
    ofDisableAlphaBlending();
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
    switch (key) {
          
        case '1':
            stateMachine.changeState("StBlank");
            break;
        case '2':
            stateMachine.changeState("StSimplePixelate");
            break;
        case '3':
            stateMachine.changeState("StCvOpDraw");
            break;
        case '4':
            stateMachine.changeState("StCvOpParticle");
            break;
        case '5':
            stateMachine.changeState("StCvOpParticlePath");
            break;
        case '6':
            stateMachine.changeState("StCvOpMesh");
            break;
        case '7':
            stateMachine.changeState("StCvOpNote");
            break;
        case '8':
            stateMachine.changeState("StCvOpDistort");
            break;
        case '9':
            stateMachine.changeState("StSakuraParticle");
            break;
        case 'q':
            stateMachine.changeState("StSoundWave");
            break;
        case 'w':
            stateMachine.changeState("StFftDrawCircle");
            break;
        case 'e':
            stateMachine.changeState("StFftDot");
            break;
        case 'r':
            stateMachine.changeState("StFftDrawBar");
            break;
        case 't':
            stateMachine.changeState("StFftBox");
            break;
        case 'y':
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