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
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
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

void OpCircle::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 15);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofSetCircleResolution(32);
    int skip = 2;
    
    ofVec2f scale = ofVec2f(ofGetWidth()/float(farneback.getWidth()), ofGetHeight()/float(farneback.getHeight()));
    ofPushMatrix();
    ofScale(scale.x, scale.y);
    //ofTranslate(0, skip / 2.0);
    
    for (int j = 0; j < farneback.getHeight()-skip; j += skip) {
        for (int i = 0; i < farneback.getWidth()-skip; i += skip) {
            ofRectangle region = ofRectangle(i, j, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region) * 2.0;
            float radius = (avrage.x + avrage.y) * 0.3;
            int n = (j * farneback.getWidth() + i) * 3;
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            ofSetColor(r, g, b, 127);
            if (abs(radius) > skip * 0.75) {
                radius = skip * 0.75;
            }
            ofCircle(i+skip/2.0, j+skip/2.0, radius);
        }
    }
    ofPopMatrix();
    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
    
    //gui.draw();
}

string OpCircle::getName(){
    return "opcircle";
}
