#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class Drop {
public:
    Drop(ofImage *image, ofImage *blur, ofVec2f position, float radius);
    ~Drop();
    void draw();
    
    float radius;
    float imageRatio;
    
    ofImage *sourceImage;
    ofImage *maskImage;
    ofImage *blurImage;
    
    ofImage bgImage;
    ofImage dropImage;
    ofImage inputImage;
    ofVec2f position;
};