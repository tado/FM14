#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class FatfontPixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    ofTrueTypeFont font;

    ofxPanel gui;
    ofxFloatSlider radius;
    ofxIntSlider srcLevel;
    ofxFloatSlider scale;
    ofTexture tex;
};