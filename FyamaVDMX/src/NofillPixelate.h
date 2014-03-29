#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class NofillPixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    
    ofxPanel gui;
    ofxFloatSlider radius;
    ofxIntSlider srcLevel;
    ofxFloatSlider circleScale;
    ofTexture tex;
};