#pragma once
#include "ofMain.h"

class GrabCamera {
public:
    void setup();
    void update();
    
    ofVideoGrabber cam;
    ofVec2f camSize;
    ofTexture camTexture;
};