#pragma once
#include "ofMain.h"

class GrabCamera {
public:
    void setup();
    void update();
    void keyPressed(int key);
    
    ofVideoGrabber cam;
    ofVec2f camSize;
    ofTexture camTexture;
};



