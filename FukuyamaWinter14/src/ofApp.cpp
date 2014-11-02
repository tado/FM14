#include "ofApp.h"

#include "SimplePixelate.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    blackmagic = new BlackmagicCapture(1920, 1080, 60.0);
    
    stateMachine.addState<SimplePixelate>();
    stateMachine.changeState("simple");
}

//--------------------------------------------------------------
void ofApp::update(){
    blackmagic->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    blackmagic->draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    blackmagic->exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
