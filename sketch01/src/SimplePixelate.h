#pragma once
#include "ofxState.h"
#include "SharedData.h"

class SimplePixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
};