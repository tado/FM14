#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"
#include "FloatingBubble.h"

class ColorBubble : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    ofPixels pixels;
    ofTexture tex;
    ofColor baseColor;
    ofColor gradStart, gradEnd;
    vector<FloatingBubble*> bubbles;
    
    ofxPanel gui;
    ofxIntSlider baseLevel;
    ofxIntSlider bubbleLevel;
    ofxIntSlider bubbleNum;
};