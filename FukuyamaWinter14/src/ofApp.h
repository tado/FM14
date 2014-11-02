#pragma once

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#include "ofMain.h"
#include "BlackmagicCapture.h"
#include "ofxStateMachine.h"
#include "SharedData.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    itg::ofxStateMachine<SharedData> stateMachine;
    BlackmagicCapture * blackmagic;
};