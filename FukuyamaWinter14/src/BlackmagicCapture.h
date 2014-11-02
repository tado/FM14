#pragma once

#include "ofMain.h"
#include "ofxBlackMagic.h"

class BlackmagicCapture {
public:
    BlackmagicCapture(int width, int height, float framerate);
    void update();
    void draw();
    void exit();
    
    int width, height;
    float framerate;
    
    ofxBlackMagic cam;
    ofTexture colorTexture;
    ofPixels colorPixels;
    ofFbo fbo;
};