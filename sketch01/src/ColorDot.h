#pragma once
#include "ofxState.h"
#include "SharedData.h"

class ColorDot : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    float radius;
};