#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"
#include "Particle.h"

class SparkleParticle {
    
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
    int imgNum;
};

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

class OpSparkle : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void stateExit();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofxCv::FlowFarneback farneback;
    
    ofPixels pixels;
    deque<SparkleParticle *> particles;
    int cvWidth, cvHeight;
    ofTexture tex;
    
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    bool OPTFLOW_FARNEBACK_GAUSSIAN;
    ofImage img;
    
    ofxPanel gui;
    ofxFloatSlider skip;
    ofxFloatSlider thresh;
    ofxIntSlider srcLevel;
    ofxFloatSlider accel;
    ofxFloatSlider hue;
    ofxFloatSlider sat;
    ofxFloatSlider br;
    ofxFloatSlider noteSize;
    ofxIntSlider num;
    ofxIntSlider max;
};