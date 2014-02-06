#include "Voronoi.h"

void Voronoi::setup(){
    bounds.set(0, 0, ofGetWidth(), ofGetHeight());
    voronoi.setBounds(bounds);
    pixels.allocate(getSharedData().camSize.x, getSharedData().camSize.y, OF_IMAGE_COLOR);
    
    for(int i=0; i<NUM; i++) {
        ofVec2f p, v;
        p = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        pos.push_back(p);
        v = ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1));
        //v = ofVec2f(0, 0);
        vel.push_back(v);
        ofColor col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        voronoi.cellColors.push_back(col);
    }
    
    generateTheVoronoi();
}

void Voronoi::update(){
    ofVec2f ratio;
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
 
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
    }

    for(int i=0; i<NUM; i++) {
        pos[i] += vel[i];
        if (pos[i].x < 0 || pos[i].x > ofGetWidth() * ratio.x) {
            vel[i].x *= -1.0;
        }
        if (pos[i].y < 0 || pos[i].y > ofGetHeight() * ratio.y) {
            vel[i].y *= -1.0;
        }
    }
}

void Voronoi::draw(){
    ofBackground(0);
    generateTheVoronoi();
    voronoi.draw();
}

void Voronoi::generateTheVoronoi() {
    ofVec2f ratio;
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    
    //voronoi.cellColors.clear();
    voronoi.clear();
    for(int i=0; i<NUM; i++) {
        ofFloatColor fcol = pixels.getColor(pos[i].x / ratio.x, pos[i].y / ratio.y);
        ofColor col = ofColor(fcol.r * 255, fcol.g * 255, fcol.b * 255);
        voronoi.cellColors.push_back(col);
        voronoi.addPoint(pos[i]);
    }
    voronoi.generateVoronoi();
}

string Voronoi::getName(){
    return "voronoi";
}