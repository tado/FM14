#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    
    grabber = new ofVideoGrabber();
    grabber->setDeviceID(2);
    grabber->initGrabber(1920, 1080/2);
    
    
    showGui = false;
    setupGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    grabber->draw(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::exit(){
    gui->saveSettings("settings.xml");
    delete gui;
}

void ofApp::setupGui(){
    vector<ofVideoDevice> devices = grabber->listDevices();
    vector<string> items;
    for (int i = 0; i < devices.size(); i++) {
        string item = devices[i].deviceName;
        items.push_back(item);
    }
    
    gui = new ofxUICanvas();
    gui->init(5, 5, 360, 100);
    gui->addSpacer();
    gui->addLabel("VIDEO DEVICE", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    ddl = gui->addDropDownList("DEVICES", items);
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(false);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    gui->loadSettings("settings.xml");
    gui->toggleVisible();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    if (e.getName() == "DEVICES") {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        vector<int> &selectID = ddlist->getSelectedIndeces();
        for (int i = 0; i < selectID.size(); i++) {
            grabber->close();
            delete grabber;
            grabber = new ofVideoGrabber();
            grabber->setDeviceID(selectID[i]);
            grabber->initGrabber(1920, 1080/2);
            cout << "Selected Video ID : " << selectID[i] << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'g') {
        gui->toggleVisible();
        showGui? ofHideCursor() : ofShowCursor();
        showGui? showGui = false : showGui = true;
    }
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
