#include "CvParticle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void CvParticle::stateEnter(){
    ofSetColor(0);
    ofRectMode(OF_RECTMODE_CORNER);
    ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
}

void CvParticle::setup() {
    // GUI
    gui.setup();
    gui.add(winsize.setup("winsize", 8, 4, 64));
    gui.add(maxLevel.setup("maxLevel", 3, 0, 8));
    gui.add(maxFeatures.setup("maxFeatures", 200, 1, 1000));
    gui.add(qualityLevel.setup("qualityLevel", 0.01, 0.001, .02));
    gui.add(minDistance.setup("minDistance", 4, 1, 16));
}

void CvParticle::update() {
    if (getSharedData().isFrameNew){
        pyrLk.setMaxFeatures(maxFeatures);
        pyrLk.setQualityLevel(qualityLevel);
        pyrLk.setMinDistance(minDistance);
        pyrLk.setWindowSize(winsize);
        pyrLk.setMaxLevel(maxLevel);
        
        // オプティカルフローを計算
        ofVideoGrabber cam = ((testApp*)ofGetAppPtr())->grabber.cam;
        pyrLk.calcOpticalFlow(cam);
        
        vector<ofVec2f> motions =  pyrLk.getMotion();
        vector<ofPoint> currents =  pyrLk.getCurrent();
        cout << "Motions num = " << motions.size() << endl;
        for (int i = 0; i < motions.size(); i++) {
            Particle p;
            p.setup(ofVec3f(currents[i].x, currents[i].y, 0), ofVec3f(motions[i].x, motions[i].y, 0));
            particles.push_back(p);
            if (particles.size() > 10000) {
                particles.pop_front();
            }
        }
    }
}

void CvParticle::draw() {
    /*
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableSmoothing();
    ofRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 15);
    ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255,127);
    ofSetLineWidth(3);
    pyrLk.draw(0,0,ofGetWidth(),ofGetHeight());
    ofDisableBlendMode();
     */
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
    gui.draw();
}

string CvParticle::getName(){
    return "particle";
}
