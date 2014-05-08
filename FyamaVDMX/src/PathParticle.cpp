#include "PathParticle.h"

PathParticle::PathParticle(ofVec3f pos){
    positions.clear();
    positions.push_back(pos);
    length = 20;
    color.setHsb(ofRandom(255), 127, 255);
    lastPos = pos;
}

void PathParticle::update(ofVec3f pos){
    float dist = ofDist(pos.x, pos.y, lastPos.x, lastPos.y);
    if (dist > 0 && dist < 12 ) {
        positions.push_back(pos);
    } else if(dist > 12) {
        positions.clear();
    }
    lastPos = pos;
    if (positions.size() > length) {
        positions.pop_front();
    }
}

void PathParticle::draw(){
    ofSetColor(color);
    ofNoFill();

    ofBeginShape();
    ofSetLineWidth(2);
    for (int i = 0; i < positions.size(); i++) {
        ofCurveVertex(positions[i].x, positions[i].y);
    }
    ofEndShape();
    ofSetLineWidth(1);
    ofFill();
}