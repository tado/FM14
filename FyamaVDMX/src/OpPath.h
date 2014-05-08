#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "PathParticle.h"

class OpPath : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowPyrLK pyrLk;
    
    float qualityLevel;
    float minDistance;
    int winSize;
    int maxLevel;
    vector<PathParticle*> particles;
    bool detectFlow;
    ofColor baseColor;
    ofPixels pixels;
    int cvWidth, cvHeight;
    
    //GUI
    ofxPanel gui;
    ofxIntSlider srcLevel;
    ofxIntSlider num;
    ofxIntSlider maxFeatures;
    ofxIntSlider minFeatures;
    ofTexture tex;
};