#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class RgbDelay : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    ofPixels pixels;
    ofTexture tex;
    deque<ofTexture> texBuffer;
    //int bufLength;
    
    ofxPanel gui;
    ofxIntSlider bufLength;
};