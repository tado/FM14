#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"

class OpCircle : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowFarneback farneback;
    
    ofxPanel gui;
    ofxFloatSlider pyrScale;
    ofxIntSlider levels;
    ofxIntSlider winsize;
    ofxIntSlider iterations;
    ofxIntSlider polyN;
    ofxFloatSlider polySigma;
    ofxToggle OPTFLOW_FARNEBACK_GAUSSIAN;
    
    ofPixels pixels;
    int cvWidth, cvHeight;
};