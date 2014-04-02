#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "OpSakura.h"

class OpSakura2 : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowFarneback farneback;
    
    ofxPanel gui;
    ofxIntSlider num;
    ofxIntSlider density;
    ofxFloatSlider windSpeed;

    
    ofPixels pixels;
    deque<SakuraParticle *> particles;
    int cvWidth, cvHeight;
    ofTexture tex;
};