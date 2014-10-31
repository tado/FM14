#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class Drop {
public:
    Drop(ofTexture *texture, ofVec2f position, float radius);
    void draw();
    
    float radius;
    ofTexture *circleTexture;
    ofVec2f position;
};