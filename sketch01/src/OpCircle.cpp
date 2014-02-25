#include "OpCircle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpCircle::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void OpCircle::setup() {
    // GUI
    gui.setup();
    gui.add(pyrScale.setup("pyrScale", .5, 0, 1));
    gui.add(levels.setup("levels", 4, 1, 8));
    gui.add(winsize.setup("winsize", 8, 4, 64));
    gui.add(iterations.setup("iterations", 2, 1, 8));
    gui.add(polyN.setup("polyN", 7, 5, 10));
    gui.add(polySigma.setup("polySigma", 1.5, 1.1, 2));
    gui.add(OPTFLOW_FARNEBACK_GAUSSIAN.setup("OPTFLOW_FARNEBACK_GAUSSIAN", false));
}

void OpCircle::update() {
    if (getSharedData().isFrameNew){
        farneback.setPyramidScale(pyrScale);
        farneback.setNumLevels(levels);
        farneback.setWindowSize(winsize);
        farneback.setNumIterations(iterations);
        farneback.setPolyN(polyN);
        farneback.setPolySigma(polySigma);
        farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
        
        ofVideoGrabber cam = ((testApp*)ofGetAppPtr())->grabber.cam;
        farneback.calcOpticalFlow(cam);
        
        getSharedData().camTexture.readToPixels(pixels);
    }
}

void OpCircle::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 5);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(31,127,255,127);
    
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    
    if (farneback.getWidth() > 0) {
        ofVec2f scale = ofVec2f(ofGetWidth()/farneback.getWidth(), ofGetHeight()/farneback.getHeight());
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        int skip = 6;
        for (int j = 0; j < farneback.getHeight()-skip; j += skip) {
            for (int i = 0; i < farneback.getWidth()-skip; i += skip) {
                ofRectangle region = ofRectangle(i, j, skip, skip);
                ofVec2f avrage = farneback.getAverageFlowInRegion(region);
                int n = ((j * camWidth + i) * 3) * camWidth / farneback.getWidth();
                unsigned char r = pixels[n];
                unsigned char g = pixels[n + 1];
                unsigned char b = pixels[n + 2];
                ofSetColor(r, g, b, 63);
                ofCircle(i, j, avrage.x + avrage.y);
            }
        }
        ofPopMatrix();
    }
    
    ofDisableBlendMode();
    //gui.draw();
}

string OpCircle::getName(){
    return "opciecle";
}
