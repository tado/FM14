#include "OpSakura.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpSakura::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //farneback.resetFlow();
}

void OpSakura::stateExit(){
    particles.clear();
    deque<SakuraParticle*>().swap(particles);
}

void OpSakura::setup() {
    gui.setup();
    gui.add(num.setup("Sakura num", 10000, 10, 20000));
    gui.add(windSpeed.setup("Sakura wind speed", 1.5, 0.0, 5.0));
}

void OpSakura::update() {
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void OpSakura::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,255);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    for (int i = 0; i < 1; i++) {
        SakuraParticle *p = new SakuraParticle();
        p->setup(ofVec3f(ofRandom(-1000, 0), ofRandom(-SCREEN_HEIGHT/2, SCREEN_HEIGHT), ofRandom(400)),
                 ofVec3f(ofRandom(windSpeed * 0.5, windSpeed * 1.5), ofRandom(0.2,1.0), 0),
                 ofColor(255,180,180));
        p->friction = 0.0;
        p->radius = 6;
        particles.push_back(p);
        if (particles.size() > num) {
            delete particles[0];
            particles.pop_front();
        }
    }
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->draw();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpSakura::getName(){
    return "sakura";
}
