#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "RectParticle.h"

class OpRedVector : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
    void setup();
    void update();
    void draw();
    void toggleRedBlue();
    string getName();
    
    ofxCv::FlowFarneback farneback;
    
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    bool OPTFLOW_FARNEBACK_GAUSSIAN;
    
    ofxPanel gui;
    ofxFloatSlider skip;
    ofxFloatSlider thresh;
    ofxIntSlider srcLevel;
    ofxFloatSlider radius;
    ofxFloatSlider accel;
    ofxFloatSlider lfoAmp;
    ofxFloatSlider lfoFreq;
    ofxFloatSlider minDist;
    ofxIntSlider num;
    ofxIntSlider max;
    
    ofPixels pixels;
    deque<RectParticle *> particles;
    int cvWidth, cvHeight;
    ofTexture tex;
    //ofImage img;
};