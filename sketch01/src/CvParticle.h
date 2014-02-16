#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "Particle.h"

class CvParticle : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofVideoGrabber camera;
    ofxCv::FlowPyrLK pyrLk;
    
    ofxPanel gui;
    ofxIntSlider winsize;
    ofxIntSlider maxLevel;
    ofxFloatSlider maxFeatures;
    ofxFloatSlider qualityLevel;
    ofxFloatSlider minDistance;
    
    deque<Particle> particles;
};