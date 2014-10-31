#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class Drop {
public:
    Drop(ofImage *image, ofImage *mask, ofVec2f position, float radius);
    void draw();
    
    float radius;
    ofImage *sourceImage;
    ofImage *maskImage;
    ofImage dropImage;
    ofVec2f position;
};