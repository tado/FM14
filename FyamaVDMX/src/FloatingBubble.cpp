#include "FloatingBubble.h"
#include "ofApp.h"

FloatingBubble::FloatingBubble(){
    position = ofVec2f(ofRandom(-radius, SCREEN_WIDTH+radius), ofRandom(-radius, SCREEN_HEIGHT+radius));
    velocity = ofVec2f(ofRandom(-2, 2), ofRandom(-2, 2));
    radius = ofRandom(10, 400);
    color.setHsb(ofRandom(255), 255, 63);
    phaseFreq = ofRandom(1.0, 5.0);
    initialPhase = ofRandom(PI);
    level = 31;
}

void FloatingBubble::update(){
    position += velocity;
    if (position.x + radius < 0){
        position.x = SCREEN_WIDTH + radius;
    }
    if (position.x - radius > SCREEN_WIDTH){
        position.x = -radius;
    }
    if (position.y + radius < 0){
        position.y = SCREEN_HEIGHT + radius;
    }
    if (position.y - radius > SCREEN_HEIGHT){
        position.y = -radius;
    }
    
    drawRadius = radius + sin(ofGetElapsedTimef() * phaseFreq + initialPhase) * radius / 4.0;
}

void FloatingBubble::draw(){
    ofColor col;
    col.setHsb(color.getHue(), 255, level);
    ofSetColor(col);
    ofSetCircleResolution(128);
    ofCircle(position.x, position.y, drawRadius);
    ofSetColor(level / 2);
    ofNoFill();
    ofCircle(position.x, position.y, drawRadius);
    ofFill();
    ofSetCircleResolution(32);
}