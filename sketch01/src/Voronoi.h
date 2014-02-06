#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxVoronoi.h"

class Voronoi : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    void generateTheVoronoi();
    string getName();
    
    ofPixels pixels;
    
    ofRectangle      bounds;
    vector <ofPoint> pts;
    ofxVoronoi       voronoi;
    vector <ofVec2f> pos;
    vector <ofVec2f> vel;
    
    static const int NUM = 1000;
};