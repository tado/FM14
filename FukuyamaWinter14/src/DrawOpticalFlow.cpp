#include "DrawOpticalFlow.h"
#include "ofApp.h"

void DrawOpticalFlow::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("OP COLOR PARTICLE");
    gui->addSpacer();
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("flow.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&DrawOpticalFlow::guiEvent);
    
    polyN = 5;
    polySigma = 1.1;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
    
    //パーティクルを生成
    for (int i = 0; i < NUM; i++){
        Particle *myParticle = new Particle();
        myParticle->friction = 0.005;
        myParticle->setup(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), ofVec2f(0, 0));
        particles.push_back(myParticle);
    }
}

void DrawOpticalFlow::update(){
    //CV params
    ofxUISlider *p = (ofxUISlider *)gui->getWidget("PYR SCALE"); pyrScale = p->getValue();
    ofxUIIntSlider *l = (ofxUIIntSlider *)gui->getWidget("LEVELS"); levels = l->getValue();
    ofxUIIntSlider *w = (ofxUIIntSlider *)gui->getWidget("WIN SIZE"); winsize = w->getValue();
    ofxUIIntSlider *it = (ofxUIIntSlider *)gui->getWidget("ITERATION"); iterations = it->getValue();
    
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    pix.resize(320, 180);
    flow.setPyramidScale(pyrScale);
    flow.setNumLevels(levels);
    flow.setWindowSize(winsize);
    flow.setNumIterations(iterations);
    flow.setPolyN(polyN);
    flow.setPolySigma(polySigma);
    flow.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    if (pix.size() > 0) {
        flow.calcOpticalFlow(pix);
    }
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void DrawOpticalFlow::draw(){
    ofSetLineWidth(3);
    ofSetColor(255, 200);
    flow.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ofSetLineWidth(1);
}

void DrawOpticalFlow::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("flow.xml");
    }
}

void DrawOpticalFlow::keyPressed(int key){
    if (key == 'g') {
        gui->toggleVisible();
    }
}

string DrawOpticalFlow::getName(){
    return "DrawOpticalFlow";
}