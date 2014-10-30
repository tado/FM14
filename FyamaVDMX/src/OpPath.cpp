#include "OpPath.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void OpPath::stateEnter(){
    ofDisableBlendMode();
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    detectFlow = false;
    baseColor.setHsb(ofRandom(255), 255, srcLevel);
}

void OpPath::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    // GUI
    gui.setup();
    gui.add(srcLevel.setup("Path srcLevel", 0, 0, 255));
    gui.add(maxFeatures.setup("Path max", 200, 0, 1000));
    gui.add(minFeatures.setup("Path min", 100, 0, 1000));
    gui.loadFromFile("settings.xml");
    
    //CV params
    qualityLevel = 0.01;
    minDistance = 2;
    winSize = 32;
    maxLevel = 3;
}

void OpPath::update() {
    pyrLk.setMaxFeatures(maxFeatures);
    pyrLk.setQualityLevel(qualityLevel);
    pyrLk.setMinDistance(minDistance);
    pyrLk.setWindowSize(winSize);
    pyrLk.setMaxLevel(maxLevel);

    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);

    pyrLk.calcOpticalFlow(pixels);
    
    // change color
    if(((ofApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor){
        int hue = baseColor.getHue();
        hue = (hue + 80) % 255;
        baseColor.setHsb(hue, 255, srcLevel);
        ((ofApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
    }
}
void OpPath::draw() {
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofClear(0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(baseColor);
    tex.loadData(((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofSetCircleResolution(32);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if (pyrLk.getWidth() > 0) {
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(pyrLk.getWidth()), SCREEN_HEIGHT / float(pyrLk.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        //ofSetColor(255);
        //pyrLk.draw();
        
        vector <ofPoint> pyrVec;
        pyrVec = pyrLk.getCurrent();
        
        ofSetColor(255, 0, 0);
        
        if (detectFlow && pyrVec.size() < minFeatures) {
            pyrLk.resetFeaturesToTrack();
            particles.clear();
            pyrVec.clear();
        }
        
        while(particles.size() > pyrVec.size()){
            particles.pop_back();
        }

        for (int i = 0; i < pyrVec.size(); i++) {
            if (i >= particles.size()) {
                particles.push_back(new PathParticle(ofVec3f(pyrVec[i].x, pyrVec[i].y, 0)));
            }
            particles[i]->update(ofVec3f(pyrVec[i].x, pyrVec[i].y, 0));
            particles[i]->draw();
            detectFlow = true;
        }
                
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpPath::getName(){
    return "oppath";
}
