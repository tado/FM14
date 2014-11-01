#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(127);
    
    // drawWidth, drawHeight
    // FullHD
    //drawWidth = 1920;
    //drawHeight = 1080;
    // 4K
    // drawWidth = 3840;
    // drawHeight = 2160;
    
    // FBO
    dropFbo.allocate(drawWidth, drawHeight, GL_RGBA);
    dropFbo.begin();
    ofClear(255,255,255, 0);
    dropFbo.end();
    
    //GUI;
    gui = new ofxUICanvas();
    gui->init(10, 10, 240, 200);
    gui->addSpacer();
    gui->addLabel("RECORDING", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->addToggle("START/STOP", false, 44, 44);
    gui->addSpacer();
    gui->addLabel("PARAMS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->addIntSlider("BLUR", 0, 100, 20);
    gui->addRangeSlider("DROP_SIZE", 1, 30, 3, 12);
    gui->addButton("DROP_CLEAR", false);
    gui->addSpacer();
    gui->addLabel("RESOLUTION", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    vector<string> names; names.push_back("HD"); names.push_back("4K");
    ofxUIRadio *radio = gui->addRadio("RESOLUTION", names, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("HD");
    gui->addSpacer();
    gui->addLabel("FPS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->addFPSSlider("FPS");
    gui->autoSizeToFitWidgets();
    //gui->loadSettings("settings.xml");
    
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    // init ofxExportImageSequence
    drawWidth = 1920;
    drawHeight = 1080;
    exp = new ofxExportImageSequence;
    exp->setup(drawWidth, drawHeight, 60);
    exp->setOverwriteSequence(true);
    exp->setAutoExit(true);
    recording = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (blurImage.getWidth() > 0) {
        dropFbo.begin();
        
        ofxUIRangeSlider *range = (ofxUIRangeSlider *)gui->getWidget("DROP_SIZE");
        float min = range->getValueLow();
        float max = range->getValueHigh();
        
        for (int i = 0; i < 3; i++) {
            Drop *d = new Drop(&sourceImage, &bgImage,
                               ofVec2f(ofRandom(drawWidth), ofRandom(drawHeight)),
                               ofRandom(min, max),
                               drawWidth, drawHeight, dropRatio);
            drops.push_back(d);
            drops[drops.size()-1]->draw();
        }
        dropFbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (blurImage.getWidth() > 0) {
        ofSetColor(255);
        blurImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        dropFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        if (recording) {
            exp->begin();
            blurImage.draw(0, 0, drawWidth, drawHeight);
            dropFbo.draw(0, 0, drawWidth, drawHeight);
            exp->end();
            
            ofSetColor(255, 0, 0);
            ofDrawBitmapString("write " + ofToString(exp->getFrameNum()) + "frames" , 12,
                               gui->getCanvasParent()->getRect()->height + 24);
        }
    }
    ofSetWindowTitle("Rainy Day");
}

void ofApp::exit(){
    for (int i = 0;  i < drops.size(); i++) {
        delete drops[i];
    }
    drops.clear();
    
    //gui->saveSettings("settings.xml");
    delete gui;
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "START/STOP"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
        recording = toggle->getValue();
        if (recording && blurImage.getWidth() > 0) {
            exp->setOutputDir(ofToString(ofGetTimestampString("%m%d%H%M%S ")));
            exp->startExport();
        }
    }

    if(name == "DROP_CLEAR"){
        for (int i = 0;  i < drops.size(); i++) {
            delete drops[i];
        }
        drops.clear();

        dropFbo.begin();
        ofClear(255,255,255, 0);
        dropFbo.end();
    }
    
    if (name == "RESOLUTION") {
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        string active = radio->getActiveName();
        if (active == "HD") {
            drawWidth = 1920;
            drawHeight = 1080;
        } else if(active == "4K"){
            drawWidth = 3840;
            drawHeight = 2160;
        }
        delete exp;
        exp = new ofxExportImageSequence;
        exp->setup(drawWidth, drawHeight, 60);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    ofShowCursor();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofShowCursor();
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
    vector<ofImage> draggedImages;
    if( dragInfo.files.size() > 0 ){
        drops.clear();
        
        // get image data
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].loadImage(dragInfo.files[k]);
        }
        draggedImages[0].resize(drawWidth, drawHeight);
        sourceImage = draggedImages[0];
        
        // Blur image
        dropRatio = 1.0;
        ofxUIIntSlider *slider = (ofxUIIntSlider *)gui->getWidget("BLUR");
        int blurSize = int(slider->getValue()) * 2 + 1;
        cv::Mat src_mat, dst_mat;
        src_mat = ofxCv::toCv(sourceImage);
        cv::GaussianBlur(src_mat, dst_mat, cv::Size(blurSize,blurSize), 0, 0);
        ofxCv::toOf(dst_mat, blurImage);
        blurImage.update();
        sourceImage.resize(1920/dropRatio, 1080/dropRatio);
        bgImage = blurImage;
        bgImage.resize(1920/dropRatio, 1080/dropRatio);
        
        dropFbo.begin();
        ofClear(255,255,255, 0);
        dropFbo.end();
        
        if (recording) {
            exp->setOutputDir(ofToString(ofGetTimestampString("%m%d%H%M%S")));
            exp->startExport();
        }

    }
}
