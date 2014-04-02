#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "Particle.h"

class OpRadial : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
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
    deque<Particle *> particles;
    int cvWidth, cvHeight;
    
    ofxPanel gui;
    ofxFloatSlider fade;
    ofxFloatSlider friction;
    ofxIntSlider num;
    ofxIntSlider max;
};