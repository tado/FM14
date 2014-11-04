#pragma once

#include "ofMain.h"
#include "ofxBlackMagic.h"

class BlackmagicCapture {
public:
    BlackmagicCapture(int width, int height, float framerate);
    void update();
    void draw();
    void exit();
    void changeInput(bool useBlackmagic);
    
    int width, height;
    float framerate;
    
    ofxBlackMagic blackmagic;
    ofVideoGrabber grabber;
    ofTexture colorTexture;
    ofPixels colorPixels;
    ofFbo fbo;
    
    bool useBlackmagic;
};