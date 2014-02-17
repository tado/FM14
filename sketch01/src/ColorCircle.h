#pragma once
#include "ofxState.h"
#include "SharedData.h"

class MyCircle {
public:
    ofColor color;
    ofVec2f pos;
    bool live;
    float bornTime;
    float radius;
    float lifeLength;
    float speed;
    MyCircle(ofColor _color, ofVec2f _pos){
        color = _color;
        pos = _pos;
        live = true;
        bornTime = ofGetElapsedTimef();
        radius = 0;
        lifeLength = ofRandom(0.1, 1.0);
        speed = 2.0;
    }
    void update(){
        radius += speed;
        if(ofGetElapsedTimef() - bornTime > lifeLength){
            live = false;
        }
    }
    void draw(){
        ofSetColor(ofColor(color));
        ofCircle(pos.x, pos.y, radius);
    }
};

class ColorCircle : public itg::ofxState<SharedData>{
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
    ofPixels pixels;
    float radius;
    vector<MyCircle *> circles;
};