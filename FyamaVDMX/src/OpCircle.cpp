#include "OpCircle.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void OpCircle::stateEnter(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void OpCircle::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    // GUI
    gui.setup();
    gui.add(skip.setup("Circle skip", 5, 1, 20));
    gui.add(fade.setup("Circle fade", 12, 0, 31));
    gui.add(hue.setup("Circle hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Circle saturation", 1.0, 0.0, 5.0));
    gui.add(br.setup("Circle brightness", 1.0, 0.0, 3.0));
    gui.loadFromFile("settings.xml");

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
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
}

void OpCircle::draw() {
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, fade);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
        
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofSetCircleResolution(64);
    //int skip = 5;
    
    ofVec2f scale = ofVec2f(SCREEN_WIDTH/float(farneback.getWidth()), SCREEN_HEIGHT/float(farneback.getHeight()));
    ofPushMatrix();
    ofScale(scale.x, scale.y);
    
    for (int j = 0; j < farneback.getHeight() - skip; j += skip) {
        for (int i = 0; i < farneback.getWidth() - skip; i += skip) {
            ofRectangle region = ofRectangle(i, j, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region) * 2.0;
            float radius = (avrage.x + avrage.y) * 2.0;
            int n = (j * farneback.getWidth() + i) * 3;
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            ofColor col = ofColor(r, g, b);
            int h = col.getHue();
            int s = col.getSaturation();
            int v = col.getBrightness();
            col.setHsb(h * hue, s * sat, v * br);
            ofSetColor(col, 63);
            
            if (abs(radius) > skip * 0.5) {
                radius = skip * 0.5;
            }
            ofCircle(i+skip/2.0, j+skip/2.0, radius);
        }
    }
    ofPopMatrix();
    ofDisableBlendMode();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpCircle::getName(){
    return "opcircle";
}
