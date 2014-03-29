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
    
    // GUI
    gui.setup();

    //CV params
    pyrScale = 0.5;
    levels = 4;
    winsize = 8;
    iterations = 2;
    polyN = 7;
    polySigma = 1.5;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
    
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
}

void OpCircle::update() {
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
}

void OpCircle::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 12);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofSetCircleResolution(64);
    int skip = 5;
    
    ofVec2f scale = ofVec2f(SCREEN_WIDTH/float(farneback.getWidth()), SCREEN_HEIGHT/float(farneback.getHeight()));
    ofPushMatrix();
    ofScale(scale.x, scale.y);
    //ofTranslate(0, skip / 2.0);
    
    for (int j = 0; j < farneback.getHeight(); j += skip) {
        for (int i = 0; i < farneback.getWidth(); i += skip) {
            ofRectangle region = ofRectangle(i, j, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region) * 2.0;
            float radius = (avrage.x + avrage.y) * 2.0;
            int n = (j * farneback.getWidth() + i) * 3;
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            ofColor col = ofColor(r, g, b);
            int hue = col.getHue();
            int sat = col.getSaturation();
            int br = col.getBrightness();
            col.setHsb(hue, sat * 2.0, br * 3.0);
            ofSetColor(col, 63);
            
            if (abs(radius) > skip * 0.5) {
                radius = skip * 0.5;
            }
            ofCircle(i+skip/2.0, j+skip/2.0, radius);
        }
    }
    ofPopMatrix();
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    // ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
}

string OpCircle::getName(){
    return "opcircle";
}
