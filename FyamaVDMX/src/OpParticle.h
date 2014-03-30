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
    
    ofxCv::FlowFarneback farneback;
    
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    bool OPTFLOW_FARNEBACK_GAUSSIAN;
    
    ofPixels pixels;
    int cvWidth, cvHeight;
    
    //GUI
    ofxPanel gui;
    ofxFloatSlider skip;
    ofxFloatSlider thresh;
    ofxIntSlider srcLevel;
    ofxIntSlider fade;
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    ofxFloatSlider alpha;
    ofxIntSlider num;
    ofTexture tex;
};