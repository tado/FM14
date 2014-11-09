#pragma once
#include "ofMain.h"

class DrawFbo {
public:
    DrawFbo();
    void draw();
    
    ofFbo fbo;
    int top, height;
};
