#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxExportImageSequence.h"
#include "ofxUI.h"

#include "Drop.h"

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
    void guiEvent(ofxUIEventArgs &e);
    
    void createCircleTexture();
    
    vector<Drop *> drops;
    ofImage sourceImage;
    ofImage blurImage;
    ofImage bgImage;
    ofImage dropImage;
    ofFbo dropFbo;
    
    float dropRatio;
    int drawWidth;
    int drawHeight;
    
    ofxExportImageSequence exp;
    
    ofxUICanvas *gui;
    bool recording;
};
