#pragma once
#include "ofMain.h"

class RectParticle {
    
public:
    void setup(ofVec3f position, ofVec3f velocity, ofColor color);
    void resetForce();
    void addForce(ofVec3f force);
    void updateForce();
    void updatePos();
    void update();
    void draw();
    
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f force;
    float friction;
    float radius;
    float lfoFreq;
    float lfoAmp;
    float lfoRand;
    ofColor color;
    int count;
};