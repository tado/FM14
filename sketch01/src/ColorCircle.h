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
        lifeLength = ofRandom(0.02,0.1);
        speed = 4.0;
    }
    void update(){
        radius += speed;
        if(ofGetElapsedTimef() - bornTime > lifeLength){
            live = false;
        }
    }
    void draw(){
        ofColor c;
        int h = color.getHue();
        int b = color.getBrightness();
        
        ofSetColor(b);
        ofCircle(pos.x, pos.y, radius * b / 180);
        
        c.setHsb(h, 31, b);
        ofSetColor(c);
        ofCircle(pos.x, pos.y, radius*1.0);
        
        ofSetColor(color);
        ofCircle(pos.x, pos.y, radius*0.8);

        /*
        c.setHsb(h, 10, b);
        ofSetColor(c);
        ofCircle(pos.x, pos.y, radius*0.5);
        
        ofSetColor(b);
        ofCircle(pos.x, pos.y, radius * b / 1000.0);
         */
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