#include "OscControl.h"
#include "ofApp.h"

void OscControl::setup(){
    receiver.setup(9000);
    
    for (int i = 0; i < 8; i++) {
        controlVal[i] = 255;
    }
}

void OscControl::update(){
    ofApp *app = (ofApp*)ofGetAppPtr();
    while(receiver.hasWaitingMessages()){
        
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/state"){
            
            int row = m.getArgAsInt32(0);
            int col = m.getArgAsInt32(1);
            
            if (row == 53 && col == 1) {
                app->stateMachine.changeState("StBlank");
            }
            if (row == 53 && col == 2) {
                app->stateMachine.changeState("StSimplePixelate");
            }
            if (row == 53 && col == 3) {
                app->stateMachine.changeState("StCvOpDraw");
            }
            if (row == 53 && col == 4) {
                app->stateMachine.changeState("StCvOpParticle");
            }
            if (row == 53 && col == 5) {
                app->stateMachine.changeState("StCvOpParticlePath");
            }
            if (row == 53 && col == 6) {
                app->stateMachine.changeState("StCvOpMesh");
            }
            if (row == 53 && col == 7) {
                app->stateMachine.changeState("StCvOpNote");
            }
            if (row == 53 && col == 8) {
                app->stateMachine.changeState("StCvOpDistort");
            }
            if (row == 54 && col == 1) {
                app->stateMachine.changeState("StSakuraParticle");
            }
            if (row == 55 && col == 1) {
                app->stateMachine.changeState("StSoundWave");
            }
            if (row == 55 && col == 2) {
                app->stateMachine.changeState("StFftDrawCircle");
            }
            if (row == 55 && col == 3) {
                app->stateMachine.changeState("StFftDot");
            }
            if (row == 55 && col == 4) {
                app->stateMachine.changeState("StFftDrawBar");
            }
            if (row == 55 && col == 5) {
                app->stateMachine.changeState("StFftBox");
            }
            if (row == 55 && col == 6) {
                app->stateMachine.changeState("StFftRibbon");
            }
        }
        
        if(m.getAddress() == "/control"){
            
            int ch = m.getArgAsInt32(0);
            int value = m.getArgAsInt32(1);
            
            for (int i = 0; i < 8; i++) {
                if (ch == i + 1) {
                    controlVal[i] = value;
                }
            }
        }
    }
}