#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    //ofSetBackgroundAuto(true);
    
    sourceImage.loadImage("source.jpg");
    sourceImage.resize(ofGetWidth(), ofGetHeight());
    
    //FBO
    dropFbo.allocate(ofGetWidth(), ofGetHeight());
    dropFbo.begin();
    ofClear(255,255,255, 0);
    dropFbo.end();
    
    // Blur image
    dropRatio = 12.0;
    cv::Mat src_mat, dst_mat;
    src_mat = ofxCv::toCv(sourceImage);
    cv::GaussianBlur(src_mat, dst_mat, cv::Size(31,31), 0, 0);
    ofxCv::toOf(dst_mat, blurImage);
    blurImage.update();
    sourceImage.resize(ofGetWidth()/dropRatio, ofGetHeight()/dropRatio);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (blurImage.getWidth() > 0) {
        dropFbo.begin();
        for (int i = 0; i < 10; i++) {
            Drop *d = new Drop(&sourceImage,
                               ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())),
                               4);
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
        blurImage.draw(0, 0);
        dropFbo.draw(0, 0);
    }
    
    ofSetColor(0);
    ofRect(0, 0, 200, 20);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 8) + "fps", 10, 10);
}

void ofApp::exit(){
    
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
    vector<ofImage> draggedImages;
    if( dragInfo.files.size() > 0 ){
        drops.clear();
        
        // get image data
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].loadImage(dragInfo.files[k]);
        }
        draggedImages[0].resize(ofGetWidth(), ofGetHeight());
        sourceImage = draggedImages[0];
        
        // Blur image
        cv::Mat src_mat, dst_mat;
        src_mat = ofxCv::toCv(sourceImage);
        cv::GaussianBlur(src_mat, dst_mat, cv::Size(31,31), 0, 0);
        ofxCv::toOf(dst_mat, blurImage);
        blurImage.update();
        
        sourceImage.resize(ofGetWidth()/8.0, ofGetHeight()/8.0);
        dropFbo.begin();
        ofClear(255,255,255, 0);
        dropFbo.end();
    }
}
