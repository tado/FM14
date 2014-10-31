#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    maskImage.loadImage("mask.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (blurImage.getWidth() > 0) {
        
        for (int i = 0; i < 20; i++) {
            Drop *d = new Drop(&sourceImage, &maskImage,
                               ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())),
                               ofRandom(1,5));
            drops.push_back(d);
        }
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
        
        // get image data
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].loadImage(dragInfo.files[k]);
        }
        draggedImages[0].resize(ofGetWidth(), ofGetHeight());
        sourceImage = draggedImages[0];
        dropImage = sourceImage;
        dropImage.resize(40, 40);
        
        // Blur image
        cv::Mat src_mat, dst_mat;
        src_mat = ofxCv::toCv(sourceImage);
        cv::GaussianBlur(src_mat, dst_mat, cv::Size(21,21), 0, 0);
        ofxCv::toOf(dst_mat, blurImage);
        blurImage.update();
        
        //createCircleTexture();
    }
}

void ofApp::createCircleTexture(){
    
    circleTexture.allocate(sourceImage.width, sourceImage.height, GL_RGB);
    circleTexture.loadData(sourceImage.getPixels(), sourceImage.width, sourceImage.height, GL_RGB);
    
    int numPts  = 64;
    float angle = 0.0;
    float step  = TWO_PI / (float)(numPts-1);
    
    
    for(int i = 0; i < numPts; i++){
        
        //get the -1 to 1 values - we will use these for drawing our pts.
        float px = cos(angle);
        float py = sin(angle);
        
        NormCirclePts.push_back( ofPoint(px, py) );
        
        //map the -1 to 1 range produced by cos/sin
        //to the dimensions of the image we need for our texture coords
        float tx = ofMap( px, -1.0, 1.0, 0, circleTexture.getWidth());
        float ty = ofMap( py, -1.0, 1.0, 0, circleTexture.getHeight());
        
        NormCircleCoords.push_back( ofPoint(tx, ty) );
        
        angle += step;
    }
}
