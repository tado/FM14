#include "Particle.h"

void Particle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    drawParticle = false;
}

void Particle::resetForce(){
    force.set(0, 0, 0);
}

void Particle::addForce(ofVec3f _force){
    force = _force;
}

void Particle::updateForce(){
    force -= velocity * friction;
}

void Particle::updatePos(){
    velocity += force;
    position += velocity;
}

void Particle::update(){
    resetForce();
    updateForce();
    updatePos();
}

void Particle::draw(){
    ofSetColor(color);
    ofCircle(position.x, position.y, position.z, radius);
}

