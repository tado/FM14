#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"

class SakuraParticle {
    
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
    ofColor color;
    ofVec3f rot;
    ofVec3f rotVel;
    int count;
};

void SakuraParticle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    rot = ofVec3f(ofRandom(360), ofRandom(360), ofRandom(360));
    rotVel = ofVec3f(ofRandom(2.0), ofRandom(2.0), ofRandom(2.0));
    count = 0;
}

void SakuraParticle::resetForce(){
    force.set(0, 0, 0);
}

void SakuraParticle::addForce(ofVec3f _force){
    force = _force;
}

void SakuraParticle::updateForce(){
    force -= velocity * friction;
}

void SakuraParticle::updatePos(){
    velocity += force;
    position += velocity;
}

void SakuraParticle::update(){
    resetForce();
    updateForce();
    updatePos();
    rot += rotVel;
}

void SakuraParticle::draw(){
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

class OpSakura : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowFarneback farneback;
    
    ofxPanel gui;
    ofxIntSlider num;
    ofxIntSlider density;
    ofxFloatSlider windSpeed;
    
    ofPixels pixels;
    deque<SakuraParticle *> particles;
    int cvWidth, cvHeight;
    ofTexture tex;
};