#pragma once
#include "ofMain.h"

class FloatingBubble {
public:
    FloatingBubble();
    void update();
    void draw();
    
    ofVec2f position;
    ofVec2f velocity;
    float radius;
    float drawRadius;
    float phaseFreq;
    float initialPhase;
    int level;
    ofColor color;
};