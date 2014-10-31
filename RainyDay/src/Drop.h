#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class Drop {
public:
    Drop(ofImage *image, ofVec2f position, float radius);
    void draw();
    
    float radius;
    ofImage *sourceImage;
    ofImage *maskImage;
    ofImage dropImage;
    ofImage inputImage;
    ofVec2f position;
};