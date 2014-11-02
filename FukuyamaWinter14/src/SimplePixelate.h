#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"

class SimplePixelate : public itg::ofxState<SharedData>{
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
};