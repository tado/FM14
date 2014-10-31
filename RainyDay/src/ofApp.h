#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
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
    
    void createCircleTexture();
    
    deque<Drop *> drops;
    ofImage sourceImage;
    ofImage blurImage;
    ofImage dropImage;
    ofImage maskImage;
    
    vector<ofPoint> NormCirclePts;
    vector<ofPoint> NormCircleCoords;
    ofTexture circleTexture;
    
    float dropRatio;
    ofFbo dropFbo;
};
