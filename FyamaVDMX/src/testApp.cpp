#include "testApp.h"

#include "Blank.h"
#include "SimplePixelate.h"
#include "LinePixelate.h"
#include "BoxPixelate.h"
#include "FatfontPixelate.h"
#include "OpCircle.h"
#include "OpParticle.h"
#include "OpVector.h"
#include "OpDistort.h"
#include "OpRadial.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);
    ofSetWindowPosition(400, 200);
    
    //syphonIO.setup(640, 480);
    syphonIO.setup(1920, 1080);
    
    
    // state machine
    stateMachine.addState<Blank>();
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<LinePixelate>();
    stateMachine.addState<BoxPixelate>();
    stateMachine.addState<FatfontPixelate>();
    stateMachine.addState<OpCircle>();
    stateMachine.addState<OpParticle>();
    stateMachine.addState<OpVector>();
    stateMachine.addState<OpDistort>();
    stateMachine.addState<OpRadial>();
    
    stateMachine.changeState("blank");
}

//--------------------------------------------------------------
void testApp::update(){
    syphonIO.update();
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case '1':
            stateMachine.changeState("blank");
            break;
        case '2':
            stateMachine.changeState("simple");
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
