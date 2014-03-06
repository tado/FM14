#pragma once
#include "ofxState.h"
#include "SharedData.h"

class BoxPixelate : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    
    float radius;
    int num;
    vector<float> angle;
    vector<float> length;
    
    ofLight light;
};