#include "TriParticle.h"

void TriParticle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    rot = ofVec3f(ofRandom(360), ofRandom(360), ofRandom(360));
    rotVel = ofVec3f(ofRandom(4.0), ofRandom(4.0), ofRandom(4.0));
    scale = 1.0;
}

void TriParticle::resetForce(){
    force.set(0, 0, 0);
}

void TriParticle::addForce(ofVec3f _force){
    force = _force;
}

void TriParticle::updateForce(){
    force -= velocity * friction;
}

void TriParticle::updatePos(){
    velocity += force;
    position += velocity;
}

void TriParticle::update(){
    resetForce();
    updateForce();
    updatePos();
    rot += rotVel;
}

void TriParticle::draw(){
    ofSetColor(color);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateX(rot.x);
    ofRotateY(rot.y);
    ofRotateZ(rot.z);
    ofSetPolyMode(OF_POLY_WINDING_NONZERO);
    ofNoFill();
    ofSetLineWidth(3);
    ofBeginShape();
    ofScale(scale * 5.0, scale * 2.0);
    ofVertex(-1, -0.5);
    ofVertex(1, 0);
    ofVertex(-1, 0.5);
    ofEndShape();
    ofFill();
    ofSetLineWidth(1);
    ofPopMatrix();
}