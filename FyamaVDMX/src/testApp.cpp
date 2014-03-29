#include "testApp.h"

#include "Blank.h"
#include "SimplePixelate.h"
#include "NofillPixelate.h"
#include "BoxPixelate.h"
#include "FatfontPixelate.h"
#include "OpCircle.h"
#include "OpParticle.h"
#include "OpBubble.h"
#include "OpSakura.h"
#include "OpRedVector.h"
#include "OpBlueVector.h"
#include "OpVector.h"
#include "OpDistort.h"
#include "OpRadial.h"
#include "Tile.h"
#include "MoveTile.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);
    //ofSetWindowPosition(400, 200);
    //syphonIO.setup(640, 480);
    syphonIO.setup(1920, 1080);
    
    // state machine
    stateMachine.addState<Blank>();
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<NofillPixelate>();
    stateMachine.addState<BoxPixelate>();
    stateMachine.addState<FatfontPixelate>();
    stateMachine.addState<OpCircle>();
    stateMachine.addState<OpParticle>();
    stateMachine.addState<OpBubble>();
    stateMachine.addState<OpSakura>();
    stateMachine.addState<OpVector>();
    stateMachine.addState<OpRedVector>();
    stateMachine.addState<OpBlueVector>();
    stateMachine.addState<OpDistort>();
    stateMachine.addState<OpRadial>();
    stateMachine.addState<Tile>();
    stateMachine.addState<MoveTile>();
    
    stateMachine.changeState("simple");
    
    // start as three head
    stateMachine.getSharedData().threeHead = true;
    
    // OSC control
    oscControl.setup();    
}

//--------------------------------------------------------------
void testApp::update(){
    syphonIO.update();
    oscControl.update();
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case '1':
            stateMachine.changeState("simple");
            break;
        case '2':
            stateMachine.changeState("nofill");
            break;
        case '3':
            stateMachine.changeState("box");
            break;
        case '4':
            stateMachine.changeState("fatfont");
            break;
        case '5':
            stateMachine.changeState("opcircle");
            break;
        case '6':
            stateMachine.changeState("opparticle");
            break;
        case '7':
            stateMachine.changeState("opvector");
            break;
        case '8':
            stateMachine.changeState("opdistort");
            break;
        case '9':
            stateMachine.changeState("opradial");
            break;
        case '0':
            stateMachine.changeState("bubble");
            break;
        case 'q':
            stateMachine.changeState("tile");
            break;
        case 'w':
            stateMachine.changeState("movetile");
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
