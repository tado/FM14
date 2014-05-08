#include "SparkleParticle.h"

void SparkleParticle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    rot = ofVec3f(ofRandom(360), ofRandom(360), ofRandom(360));
    rotVel = ofVec3f(ofRandom(2.0), ofRandom(2.0), ofRandom(2.0));
    count = 0;
    imgNum = ofRandom(4);
}

void SparkleParticle::resetForce(){
    force.set(0, 0, 0);
}

void SparkleParticle::addForce(ofVec3f _force){
    force = _force;
}

void SparkleParticle::updateForce(){
    force -= velocity * friction;
}

void SparkleParticle::updatePos(){
    velocity += force;
    position += velocity;
}

void SparkleParticle::update(){
    resetForce();
    updateForce();
    updatePos();
    rot += rotVel;
}

void SparkleParticle::draw(){
    ofSetColor(color);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateX(rot.x);
    ofRotateY(rot.y);
    ofRotateZ(rot.z);
    ofEllipse(0, 0, radius, radius * 1.4);
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
}