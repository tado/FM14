#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    
    ofVideoGrabber grabber;
    //ofShader shader;
    
    int deviceId;
    int topMargin;
    int gridWidth;
    int gridHeight;
    ofTrueTypeFont font;
    
    bool showGui;
    bool testPattern;
    vector<string> items;
    ofxUICanvas *gui;
    ofxUIDropDownList *ddl;
};
