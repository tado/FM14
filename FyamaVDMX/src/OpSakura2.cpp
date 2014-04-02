#include "OpSakura2.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpSakura2::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //farneback.resetFlow();
}

void OpSakura2::stateExit(){
    particles.clear();
    deque<SakuraParticle*>().swap(particles);
}

void OpSakura2::setup() {
    gui.setup();
    gui.add(num.setup("Sakura2 num", 10000, 10, 20000));
    gui.add(density.setup("Sakura2 density", 20, 40, 1));
    gui.add(windSpeed.setup("Sakura2 wind speed", 1.5, 0.0, 5.0));
}

void OpSakura2::update() {
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void OpSakura2::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,255);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (ofGetFrameNum() % density == 0){
        for (int i = 0; i < 4; i++) {
            SakuraParticle *p = new SakuraParticle();
            p->setup(ofVec3f(ofRandom(SCREEN_WIDTH/3 - 100, SCREEN_WIDTH/3), ofRandom(-SCREEN_HEIGHT/2, SCREEN_HEIGHT), ofRandom(400)),
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
    }
    
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i]->position.x > SCREEN_WIDTH/3 && particles[i]->position.x < SCREEN_WIDTH/3 * 2 ) {
            particles[i]->draw();
        }
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpSakura2::getName(){
    return "sakura2";
}
