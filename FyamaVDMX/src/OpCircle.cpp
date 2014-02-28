#include "OpCircle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpCircle::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
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
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    
    ((testApp*)ofGetAppPtr())->syphonIO.texture.readToPixels(pixels);
    farneback.calcOpticalFlow(pixels);
}

void OpCircle::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 7);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    
    ofSetCircleResolution(32);
    
    if (farneback.getWidth() > 0) {
        ofVec2f scale = ofVec2f(ofGetWidth()/float(farneback.getWidth()), ofGetHeight()/float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        int skip = 4;
        for (int j = 0; j < farneback.getHeight()-skip; j += skip) {
            for (int i = 0; i < farneback.getWidth()-skip; i += skip) {
                ofRectangle region = ofRectangle(i, j, skip, skip);
                ofVec2f avrage = farneback.getAverageFlowInRegion(region) * 2.0;
                float radius = avrage.x + avrage.y;
                int n = ((j * camWidth + i) * 3) * camWidth / farneback.getWidth();
                unsigned char r = pixels[n];
                unsigned char g = pixels[n + 1];
                unsigned char b = pixels[n + 2];
                ofSetColor(r, g, b);
                if (abs(radius) > skip / 2.0) {
                    radius = skip / 2.0;
                }
                ofCircle(i+skip/2.0, j+skip/2.0, radius);
            }
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    //gui.draw();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpCircle::getName(){
    return "opcircle";
}
