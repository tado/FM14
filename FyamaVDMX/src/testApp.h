#pragma once

#define SCREEN_WIDTH 3840
#define SCREEN_HEIGHT 720

#include "ofMain.h"
#include "ofxSyphon.h"
#include "SyphonIO.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "OscControl.h"

class testApp : public ofBaseApp{
    
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
    
    SyphonIO syphonIO;
    itg::ofxStateMachine<SharedData> stateMachine;
    
    OscControl oscControl;
};

