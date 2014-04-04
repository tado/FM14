#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"

class OpVbo : public itg::ofxState<SharedData>{
    
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
    
    int cvWidth, cvHeight;
    ofPixels pixels;
    
    ofxPanel gui;
    //ofxIntSlider skip;
    ofxFloatSlider accel;
    ofxFloatSlider fade;
    ofxFloatSlider friction;
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    ofxFloatSlider radius;
    
    static const int NUM = 100000;
    ofVbo vbo;
    ofVec3f verts[NUM];
    ofFloatColor colors[NUM];
    ofVec3f vels[NUM];
    ofVec3f velocity[NUM];
    ofVec3f force[NUM];
    //float friction;
    bool initPos;
};