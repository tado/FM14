#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "Particle.h"

class OpColorParticle : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void guiEvent(ofxUIEventArgs &e);
    void keyPressed(int key);
    
    ofPixels pixels;
    ofTexture tex;
    ofxUICanvas *gui;
    
    // CV
    ofxCv::FlowFarneback flow;
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    float flowScale;
    
    // Particle
    vector<Particle> particles;
    static const int NUM = 20000;
    ofVboMesh mesh;
};