#pragma once
#include "ofMain.h"
#include "ofxPostProcessing.h"

class DrawFbo {
public:
    DrawFbo();
    void draw();
    
    ofFbo fbo;
    int top, width, height;
    ofxPostProcessing post;
};
