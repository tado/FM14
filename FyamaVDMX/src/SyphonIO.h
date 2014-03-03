#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

class SyphonIO {
public:
    void setup(int width, int height);
    void update();
    void draw();
    
    // client
    ofxSyphonClient client;
    ofFbo fboSyphonIn;
    
    // server
    ofxSyphonServerDirectory dir;
    ofxSyphonServer server;
    ofFbo fboSyphonOut;
    
    int width, height;
    ofTexture texture;
    
    //GUI
    ofxPanel gui;
    ofxToggle fitScreen;
    ofxIntSlider screenPos;
};