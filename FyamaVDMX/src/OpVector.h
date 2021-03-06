#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "Particle.h"

class OpVector : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowFarneback farneback;

    ofPixels pixels;
    deque<Particle *> particles;
    int cvWidth, cvHeight;
    ofTexture tex;
    
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
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    ofxIntSlider num;
    ofxIntSlider max;
};