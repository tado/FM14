#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class BoxPixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    
    int num;
    vector<float> angle;
    vector<float> length;
    
    ofxPanel gui;
    ofxFloatSlider radius;
    ofxIntSlider srcLevel;
    ofxFloatSlider rectScale;
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    ofTexture tex;
};