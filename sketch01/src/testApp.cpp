#include "testApp.h"
#include "SimplePixelate.h"
#include "LinePixelate.h"
#include "FatfontPixelate.h"
#include "OpticalFlow.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetBackgroundAuto(false);
    
    stateMachine.getSharedData().camSize.x = 320;
    stateMachine.getSharedData().camSize.y = 240;
    
    //StateMachine
    stateMachine.addState<SimplePixelate>();
    stateMachine.addState<LinePixelate>();
    stateMachine.addState<FatfontPixelate>();
    stateMachine.addState<OpticalFlow>();
    //stateMachine.addState<Voronoi>();
    stateMachine.changeState("simple");
    
    grabber.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    //update grabber and get texture
    grabber.update();
    stateMachine.getSharedData().camTexture = grabber.cam.getTextureReference();
    stateMachine.getSharedData().isFrameNew = grabber.cam.isFrameNew();
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofSetColor(ofColor::white);
    //stateMachine.getSharedData().camTexture.draw(0, 0, 160, 120);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case '1':
            stateMachine.changeState("simple");
            break;
        case '2':
            stateMachine.changeState("fatfont");
            break;
        case '3':
            stateMachine.changeState("line");
            break;
        case '4':
            stateMachine.changeState("optical");
            break;
            
        case 's':
            grabber.cam.videoSettings();
            break;

        //case '4':
        //    stateMachine.changeState("voronoi");
        //    break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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