#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxPostProcessing.h"

class FatfontPixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    ofTrueTypeFont font;
    float radius;
};