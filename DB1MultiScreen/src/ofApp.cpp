#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    showGui = false;
    testPattern = false;
    
    // shader.load("deinterlace.frag");
    
    font.loadFont("NotoSans-Bold.ttf", 100);
    
    gui.setup();
    gui.add(deviceId.setup("device ID", 2, 0, 12));
    gui.add(topMargin.setup("topMargin", 135, 0, 270));
    gui.add(gridWidth.setup("grid width", 480, 1, 960));
    gui.add(gridHeight.setup("grid height", 270, 1, 540));
    gui.loadFromFile("settings.xml");
    
    video.listDevices();
    video.setDeviceID(deviceId);
    video.initGrabber(1920, 1080);
    
    deviceId.addListener(this, &ofApp::deviceIdChanged);
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int xdiv = 4;
    int ydiv = 3;
    
    ofPushMatrix();
    // shader.begin();
    
    ofTranslate(0, topMargin);
    
    if (testPattern) {
        // Test Pattern
        for (int j = 0; j < ydiv; j++) {
            for (int i = 0; i < xdiv; i++) {
                if ((i + j) % 2 == 0) {
                    ofSetColor(255, 0, 0);
                } else {
                    ofSetColor(0, 0, 255);
                }
                ofRect(gridWidth * i, gridHeight * j, gridWidth, gridHeight);
                ofSetColor(255);
                string msg = ofToString(i+1, 0) + "-" + ofToString(j+1, 0);
                font.drawString(msg, gridWidth * i + gridWidth / 3.5, gridHeight * j + gridHeight / 1.5);
            }
        }
    } else {
        // draw video grid
        ofSetColor(255);
        for (int j = 0; j < ydiv; j++) {
            for (int i = 0; i < xdiv; i++) {
                video.draw(gridWidth * i, gridHeight * j, gridWidth, gridHeight);
            }
        }
    }
    //shader.end();
    ofPopMatrix();
    
    if (showGui) {
        gui.draw();
    }
}

void ofApp::deviceIdChanged(int & deviceId){
    video.close();
    video.setDeviceID(deviceId);
    video.initGrabber(1920, 1080);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'g') {
        showGui? ofHideCursor() : ofShowCursor();
        showGui? showGui = false : showGui = true;
    }
    if (key == 't') {
        testPattern? testPattern = false : testPattern = true;
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
