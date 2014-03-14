#include "Ribbon.h"

void Ribbon::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    length = 4;
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

void Ribbon::update(){
    resetForce();
    updateForce();
    updatePos();
    verts.push_back(position);
    if (verts.size() > length) {
        verts.pop_front();
    }
}

void Ribbon::draw(){
    ofSetColor(color);
    //ofCircle(position.x, position.y, position.z, radius);
    ofSetPolyMode(OF_POLY_WINDING_NONZERO);
    if (radius > 4) {
        radius = 4;
    }
    ofSetLineWidth(radius);
    if (verts.size() > 2) {
        //ofLine(verts[0].x, verts[0].y, verts[0].z,
        //   verts[verts.size()-1].x, verts[verts.size()-1].y, verts[verts.size()-1].z);
        ofSetColor(255);
        ofCircle(verts[verts.size()-1], radius*0.11);
    }
}

