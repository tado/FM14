#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class StKaleidoscope : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    
    float fftSum;
    ofEasyCam cam;
    ofxPostProcessing post;
    KaleidoscopePass::Ptr kaleido;
    NoiseWarpPass::Ptr noise;
    BloomPass::Ptr bloom;
};