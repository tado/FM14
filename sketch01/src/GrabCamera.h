#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class GrabCamera {
public:
    void setup();
    void update();
    void draw();
    
    ofVideoGrabber cam;
    ofVec2f camSize;
    ofTexture camTexture;
};



