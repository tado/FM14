#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    sourceImage.loadImage("source.jpg");
    sourceImage.resize(ofGetWidth(), ofGetHeight());
    
    // Blur image
    cv::Mat src_mat, dst_mat;
    src_mat = ofxCv::toCv(sourceImage);
    cv::GaussianBlur(src_mat, dst_mat, cv::Size(31,31), 0, 0);
    ofxCv::toOf(dst_mat, blurImage);
    blurImage.update();
}

//--------------------------------------------------------------
void ofApp::update(){
    if (blurImage.getWidth() > 0) {        
        // for (int i = 0; i < 20; i++) {
            Drop *d = new Drop(&sourceImage,
                               ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())),
                               ofRandom(3, 12));
            drops.push_back(d);
        //}
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (blurImage.getWidth() > 0) {
        blurImage.draw(0, 0);
        
        for (int i = 0; i < drops.size(); i++) {
            drops[i]->draw();
        }
    }
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 8) + "fps", 10, 10);
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
    }
}
