#include "testApp.h"

#include "Blank.h"
#include "SimplePixelate.h"
#include "LinePixelate.h"
#include "OpCircle.h"
#include "OpParticle.h"
#include "OpVector.h"
#include "FatfontPixelate.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);
    
    syphonIO.setup(640, 480);
    
    // state machine
    stateMachine.addState<Blank>();
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<LinePixelate>();
    stateMachine.addState<FatfontPixelate>();
    stateMachine.addState<OpCircle>();
    stateMachine.addState<OpParticle>();
    stateMachine.addState<OpVector>();
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
            stateMachine.changeState("line");
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
