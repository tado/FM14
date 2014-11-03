#include "StCvOpParticle.h"
#include "ofApp.h"

string StCvOpParticle::getName(){
    return "StCvOpParticle";
}

void StCvOpParticle::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("OPTICAL PARTICLE");
    gui->addSpacer();
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addToggle("GAUSS", false);
    gui->addSpacer();
    gui->addSlider("RADIUS", 20, 200, 60);
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("flow.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    ofAddListener(gui->newGUIEvent,this,&StCvOpParticle::guiEvent);
    
    /*
    //パーティクルを生成
    for (int i = 0; i < NUM; i++){
        Particle *myParticle = new Particle();
        myParticle->friction = 0.005;
        myParticle->setup(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), ofVec2f(0, 0), ofColor(255));
        particles.push_back(myParticle);
    }
     */
}

void StCvOpParticle::update(){
    //CV params
    ofxUISlider *p = (ofxUISlider *)gui->getWidget("PYR SCALE"); pyrScale = p->getValue();
    ofxUIIntSlider *l = (ofxUIIntSlider *)gui->getWidget("LEVELS"); levels = l->getValue();
    ofxUIIntSlider *w = (ofxUIIntSlider *)gui->getWidget("WIN SIZE"); winsize = w->getValue();
    ofxUIIntSlider *it = (ofxUIIntSlider *)gui->getWidget("ITERATION"); iterations = it->getValue();
    ofxUIToggle *gs = (ofxUIToggle *)gui->getWidget("GAUSS"); OPTFLOW_FARNEBACK_GAUSSIAN = gs->getValue();
    polyN = 7;
    polySigma = 1.5;
    
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    pix.resize(160, 90);
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
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpParticle::draw(){
    int skip = 10;
    int max = 500;
    int thresh = 5;
    float radius = 0.3;
    float accel = 0.3;
    float hue = 1.0;
    float sat = 1.0;
    float br = 1.0;
    
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    int camWidth = pix.getWidth();
    int camHeight = pix.getHeight();
    
    if (flow.getWidth() > 0) {
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(flow.getWidth()), SCREEN_HEIGHT / float(flow.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < max; i++) {
            int x = ofRandom(flow.getWidth()-skip);
            int y = ofRandom(flow.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = flow.getAverageFlowInRegion(region);
            
            if (average.length() > thresh) {
                average = ofVec2f(0, 0);
            }
            
            if (abs(average.x) + abs(average.y) > 0.5) {
                int n = ((y * camWidth + x) * 3) * camWidth / flow.getWidth();
                unsigned char r = pix[n];
                unsigned char g = pix[n + 1];
                unsigned char b = pix[n + 2];
                
                ofColor col = ofColor(r, g, b);
                int h = col.getHue();
                int s = col.getSaturation();
                int v = col.getBrightness();
                col.setHsb(h * hue, s * sat, v * br);
                
                Particle *p = new Particle();
                p->setup(ofVec2f(x + ofRandom(skip), y + ofRandom(skip)), ofVec2f(average.x * accel, average.y * accel), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                //if (abs(p->radius) > skip) {
                //    p->radius = skip;
                //}
                particles.push_back(p);

                while (particles.size() > max) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw();
        }
        ofDisableBlendMode();
        ofPopMatrix();
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpParticle::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("flow.xml");
    }
}

void StCvOpParticle::stateExit(){
    gui->setVisible(false);
}