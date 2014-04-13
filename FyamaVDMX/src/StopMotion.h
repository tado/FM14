#pragma once
#include "ofxState.h"
#include "SharedData.h"

class StopMotion : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    ofPixels pixels;
    ofTexture tex;
    ofTexture drawTex;
    bool lastState;
};