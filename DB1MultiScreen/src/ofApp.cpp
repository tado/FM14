#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    showGui = false;
    testPattern = false;
    
    topMargin = 135;
    gridWidth = 480;
    gridHeight = 270;
    
    font.loadFont("NotoSans-Bold.ttf", 100);
    
    showGui = false;
    
    grabber.initGrabber(1920, 1080);
    grabber.setDeviceID(2);
    setupGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int xdiv = 4;
    int ydiv = 3;
    
    ofPushMatrix();
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
                string msg = ofToString(j+1, 0) + "-" + ofToString(i+1, 0);
                font.drawString(msg, gridWidth * i + gridWidth / 3.5, gridHeight * j + gridHeight / 1.5);
            }
        }
    } else {
        // draw video grid
        ofSetColor(255);
        for (int j = 0; j < ydiv; j++) {
            for (int i = 0; i < xdiv; i++) {
                grabber.draw(gridWidth * i, gridHeight * j, gridWidth, gridHeight);
            }
        }
    }

    ofPopMatrix();
}


void ofApp::setupGui(){
    vector<ofVideoDevice> devices = grabber.listDevices();
    for (int i = 0; i < devices.size(); i++) {
        string item = devices[i].deviceName;
        items.push_back(item);
    }
    
    gui = new ofxUICanvas();
    gui->setWidth(360);
    gui->setHeight(54);
    gui->setPosition(5, 5);
    gui->addLabel("VIDEO SETTINGS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    ddl = gui->addDropDownList("DEVICES", items);
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(false);
    gui->autoSizeToFitWidgets();
    gui->setDrawWidgetPadding(true);
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    gui->loadSettings("settings.xml");
    gui->toggleVisible();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    if (e.getName() == "DEVICES") {
        grabber.close();
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for (int i = 0; i < items.size(); i++) {
            if(selected.size() > 0 && items[i] == selected[0]->getName()){
                grabber.setDeviceID(i);
                cout << "Selected Device: " << selected[0]->getName() << endl;
                cout << "Selected ID: " << i << endl;
            }
        }
        grabber.initGrabber(1920, 1080);
    }
}

void ofApp::exit(){
    gui->saveSettings("settings.xml");
    delete gui;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'g') {
        gui->toggleVisible();
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
