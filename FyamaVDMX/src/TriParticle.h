#pragma once
#include "ofMain.h"

class TriParticle {
    
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
    ofVec3f rot;
    ofVec3f rotVel;
    float friction;
    float scale;
    ofColor color;
};