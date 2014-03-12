#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxState.h"
#include "SharedData.h"

class OpDistort : public itg::ofxState<SharedData>{
    
public:
    void stateEnter();
    void setup();
    void update();
    void draw();
    string getName();
    
	ofxCv::FlowFarneback flow;
	ofMesh mesh;
	float stepSize, xSteps, ySteps;
    
    ofPixels pixels;
    int cvWidth, cvHeight;
    int camWidth, camHeight;
    ofVec2f cvScale;
    ofTexture tex;
    vector<ofVec3f> verts;
};