#include "OpticalFlow.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpticalFlow::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void OpticalFlow::setup() {
    // GUI
    gui.setup();
    gui.add(pyrScale.setup("pyrScale", .5, 0, 1));
    gui.add(levels.setup("levels", 4, 1, 8));
    gui.add(winsize.setup("winsize", 8, 4, 64));
    gui.add(iterations.setup("iterations", 2, 1, 8));
    gui.add(polyN.setup("polyN", 7, 5, 10));
    gui.add(polySigma.setup("polySigma", 1.5, 1.1, 2));
    gui.add(OPTFLOW_FARNEBACK_GAUSSIAN.setup("OPTFLOW_FARNEBACK_GAUSSIAN", false));
    
    gui.add(useFarneback.setup("useFarneback", true));
    gui.add(winSize.setup("winSize", 32, 4, 64));
    gui.add(maxLevel.setup("maxLevel", 3, 0, 8));
    
    gui.add(maxFeatures.setup("maxFeatures", 200, 1, 1000));
    gui.add(qualityLevel.setup("qualityLevel", 0.01, 0.001, .02));
    gui.add(minDistance.setup("minDistance", 4, 1, 16));
    
    curFlow = &farneback;
}

void OpticalFlow::update() {
    if (getSharedData().isFrameNew){
        if(useFarneback) { // Farnebackの密なオプティカルフロー
            curFlow = &farneback;
            farneback.setPyramidScale(pyrScale);
            farneback.setNumLevels(levels);
            farneback.setWindowSize(winsize);
            farneback.setNumIterations(iterations);
            farneback.setPolyN(polyN);
            farneback.setPolySigma(polySigma);
            farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
        } else { // 画像ピラミッドを利用した、疎なオプティカルフロー
            curFlow = &pyrLk;
            pyrLk.setMaxFeatures(maxFeatures);
            pyrLk.setQualityLevel(qualityLevel);
            pyrLk.setMinDistance(minDistance);
            pyrLk.setWindowSize(winSize);
            pyrLk.setMaxLevel(maxLevel);
        }
        // オプティカルフローを計算
        ofVideoGrabber cam = ((testApp*)ofGetAppPtr())->grabber.cam;
        curFlow->calcOpticalFlow(cam);
    }
}

void OpticalFlow::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofEnableSmoothing();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 15);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    //ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(31,127,255,127);
    ofSetLineWidth(3);
    curFlow->draw(0,0,ofGetWidth(),ofGetHeight());
    ofDisableBlendMode();
    //ofDisableSmoothing();

    //gui.draw();
}

string OpticalFlow::getName(){
    return "optical";
}
