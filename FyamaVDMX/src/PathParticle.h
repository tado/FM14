#pragma once
#include "ofMain.h"

class PathParticle {
    
public:
    PathParticle(ofVec3f position);
    void update(ofVec3f position);
    void draw();
    
    deque<ofVec3f> positions;
    ofVec3f lastPos;
    int length;
    ofColor color;
};