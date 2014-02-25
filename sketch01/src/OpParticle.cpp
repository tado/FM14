#include "OpParticle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpParticle::stateEnter(){
    ofDisableBlendMode();
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void OpParticle::setup() {
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

void OpParticle::update() {
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

void OpParticle::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 7);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());

    ofSetCircleResolution(32);
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    
    if (farneback.getWidth() > 0) {
        ofVec2f scale = ofVec2f(ofGetWidth()/farneback.getWidth(), ofGetHeight()/farneback.getHeight());
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        int skip = 1;
        for (int i = 0; i < 2000; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region);
            float radius = avrage.x + avrage.y;
            int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            ofSetColor(r, g, b);
            ofCircle(x + ofRandom(-skip,skip), y + ofRandom(-skip,skip), radius);
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
}

string OpParticle::getName(){
    return "opparticle";
}
