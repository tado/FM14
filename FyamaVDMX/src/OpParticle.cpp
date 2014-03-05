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
    cvWidth = 240;
    cvHeight = 45;
    
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
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    //((testApp*)ofGetAppPtr())->syphonIO.texture.readToPixels(pixels);
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
}

void OpParticle::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 16);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofSetCircleResolution(32);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        
        ofVec2f scale = ofVec2f(ofGetWidth()/float(farneback.getWidth()), ofGetHeight()/float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        ofTranslate(0, skip);

        for (int i = 0; i < 5000; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region);
            float radius = (avrage.x + avrage.y) * 0.5;
            int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            ofSetColor(r, g, b);
            if (abs(radius) > skip) {
                radius = skip;
            }
            ofCircle(x + ofRandom(-skip,skip), y + ofRandom(-skip,skip), radius);
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpParticle::getName(){
    return "opparticle";
}
