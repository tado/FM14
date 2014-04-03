#include "RectParticle.h"

void RectParticle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    count = 0;
    lfoFreq = 0.05;
    lfoRand = ofRandom(1.0);
}

void RectParticle::resetForce(){
    force.set(0, 0, 0);
}

void RectParticle::addForce(ofVec3f _force){
    force = _force;
}

void RectParticle::updateForce(){
    force -= velocity * friction;
}

void RectParticle::updatePos(){
    velocity += force;
    position += velocity;
    position.z = sin(lfoFreq * lfoRand * count) * lfoAmp;
    count++;
}

void RectParticle::update(){
    resetForce();
    updateForce();
    updatePos();
}

void RectParticle::draw(){
    ofSetColor(color);
    ofCircle(position.x, position.y, position.z, radius);
}

