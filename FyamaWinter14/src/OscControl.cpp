#include "OscControl.h"
#include "ofApp.h"


string stateMatrix[5][8] = {
    {"StFftDrawCircle", "StFftBox", "StFftSphereRibbon"},
    {"StFftDrawBar", "StFftSphereStripe", "StFftDistort", "StFftRibbon"},
    {"StSoundWave", "StFftDistortStripe", "StFftSphereStripe", "StFftSphereDistort", "StKaleidoscope"},
    {"StSimplePixelate", "StTrianglePixelate", "StCvOpDraw", "StCvOpParticle", "StCvOpParticlePath", "StCvOpNote", "StCvOpMesh", "StCvOpDistort"},
    {"StRgbDelay", "StFftDot", "StSakuraParticle", "", "", "", "", "StBlank"}
};

void OscControl::setup(){
    receiver.setup(9000);
    for (int i = 0; i < 8; i++) {
        controlVal[i] = 127;
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

            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 5; i++) {
                    if ((i == row - 53) && (j == col - 1) && stateMatrix[i][j].length() > 0) {
                        //cout << "Select State : " << stateMatrix[i][j] << endl;
                        app->stateMachine.changeState(stateMatrix[i][j]);
                    }
                }
            }
            
            /*
            if (row == 53 && col == 1) {
                app->stateMachine.changeState("StBlank"); x
            }
            if (row == 53 && col == 2) {
                app->stateMachine.changeState("StSimplePixelate"); x
            }
            if (row == 53 && col == 3) {
                app->stateMachine.changeState("StCvOpDraw"); x
            }
            if (row == 53 && col == 4) {
                app->stateMachine.changeState("StCvOpParticle"); x
            }
            if (row == 53 && col == 5) {
                app->stateMachine.changeState("StCvOpParticlePath"); x
            }
            if (row == 53 && col == 6) {
                app->stateMachine.changeState("StCvOpMesh"); x
            }
            if (row == 53 && col == 7) {
                app->stateMachine.changeState("StCvOpNote"); x
            }
            if (row == 53 && col == 8) {
                app->stateMachine.changeState("StCvOpDistort"); x
            }
            if (row == 54 && col == 1) {
                app->stateMachine.changeState("StSakuraParticle"); x
            }
            if (row == 54 && col == 2) {
                app->stateMachine.changeState("StRgbDelay"); x
            }
            if (row == 54 && col == 3) {
                app->stateMachine.changeState("StTrianglePixelate");
            }
            if (row == 55 && col == 1) {
                app->stateMachine.changeState("StSoundWave"); x
            }
            if (row == 55 && col == 2) {
                app->stateMachine.changeState("StFftDrawCircle"); x
            }
            if (row == 55 && col == 3) {
                app->stateMachine.changeState("StFftDot");
            }
            if (row == 55 && col == 4) {
                app->stateMachine.changeState("StFftDrawBar"); x
            }
            if (row == 55 && col == 5) {
                app->stateMachine.changeState("StFftBox"); x
            }
            if (row == 55 && col == 6) {
                app->stateMachine.changeState("StFftRibbon"); x
            }
            if (row == 55 && col == 7) {
                app->stateMachine.changeState("StFftDistort"); x
            }
            if (row == 55 && col == 8) {
                app->stateMachine.changeState("StFftDistortStripe"); x
            }
            if (row == 56 && col == 1) {
                app->stateMachine.changeState("StFftSphereStripe"); x
            }
            if (row == 56 && col == 2) {
                app->stateMachine.changeState("StFftSphereDistort"); x
            }
            if (row == 56 && col == 3) {
                app->stateMachine.changeState("StKaleidoscope"); x
            }
             */
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