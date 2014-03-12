#include "Ribbon.h"

void Ribbon::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
}

void Ribbon::resetForce(){
    force.set(0, 0, 0);
}

void Ribbon::addForce(ofVec3f _force){
    force = _force;
}

void Ribbon::updateForce(){
    force -= velocity * friction;
}

void Ribbon::updatePos(){
    velocity += force;
    position += velocity;
}

void Ribbon::draw(){
    ofSetColor(color);
    ofCircle(position.x, position.y, position.z, radius);
}

