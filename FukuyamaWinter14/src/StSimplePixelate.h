#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"

class StSimplePixelate : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofPixels pixels;        
    ofTexture tex;
    ofxUICanvas *gui;
};