#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class PhaseShift : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    ofPixels pixels;
    ofTexture tex;
    deque<ofTexture> texBuffer;
    
    ofxPanel gui;
    ofxIntSlider sliceHeight;
    ofxIntSlider throughLevel;
    ofColor baseColor;
    int bufLength;
    ofPixels readPix;
    ofTexture drawTex;
};