#include "testApp.h"
#include "SimplePixelate.h"
#include "OpCircle.h"
#include "OpVector.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);
    
    syphonIO.setup(640, 512);
    
    // state machine
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<OpCircle>();
    stateMachine.addState<OpVector>();
    stateMachine.changeState("simple");
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
            stateMachine.changeState("simple");
            break;
        case '2':
            stateMachine.changeState("opcircle");
            break;
        case '3':
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
