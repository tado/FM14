#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class SimplePixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    
    //GUI
    ofxPanel gui;
    ofxFloatSlider radius;
    ofxIntSlider srcLevel;
    ofxFloatSlider circleScale;
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    
    ofTexture tex;
};