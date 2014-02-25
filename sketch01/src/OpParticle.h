#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"

class OpParticle : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofVideoGrabber camera;
    ofxCv::FlowFarneback farneback;
    
    ofxPanel gui;
    ofxFloatSlider pyrScale;
    ofxIntSlider levels;
    ofxIntSlider winsize;
    ofxIntSlider iterations;
    ofxIntSlider polyN;
    ofxFloatSlider polySigma;
    ofxToggle OPTFLOW_FARNEBACK_GAUSSIAN;
    
    ofxToggle useFarneback;
    ofxIntSlider winSize;
    ofxIntSlider maxLevel;
    
    ofxFloatSlider maxFeatures;
    ofxFloatSlider qualityLevel;
    ofxFloatSlider minDistance;
    
    ofPixels pixels;
};