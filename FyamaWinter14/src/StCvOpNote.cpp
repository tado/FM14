#include "StCvOpNote.h"
#include "ofApp.h"

string StCvOpNote::getName(){
    return "StCvOpNote";
}

void StCvOpNote::setup(){
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
    gui->addIntSlider("SKIP", 1, 50, 10);
    gui->addIntSlider("MAX", 0, 200, 100);
    gui->addSlider("THRESH", 0, 2.0, 1.0);
    gui->addSlider("RADIUS", 0, 10.0, 0.2);
    gui->addSlider("FRICTION", 0, 0.1, 0.001);
    gui->addSlider("ACCEL", 0, 4.0, 1.2);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StCvOpNote.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StCvOpNote::guiEvent);
    
    img[0].loadImage("note0.png");
    img[1].loadImage("note1.png");
    img[2].loadImage("note2.png");
    img[3].loadImage("note3.png");
    
    post.init(ofGetWidth(), ofGetHeight());
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StCvOpNote::update(){
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

void StCvOpNote::draw(){
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUIIntSlider *gmax = (ofxUIIntSlider *)gui->getWidget("MAX"); int max = gmax->getValue();
    ofxUISlider *gthresh = (ofxUISlider *)gui->getWidget("THRESH"); float thresh = gthresh->getValue();
    ofxUISlider *gradius = (ofxUISlider *)gui->getWidget("RADIUS"); float radius = gradius->getValue();
    ofxUISlider *gfriction = (ofxUISlider *)gui->getWidget("FRICTION"); float friction = gfriction->getValue();
    ofxUISlider *gaccel = (ofxUISlider *)gui->getWidget("ACCEL"); float accel = gaccel->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    int camWidth = SCREEN_WIDTH;
    int camHeight = SCREEN_HEIGHT;
    pix.resize(camWidth, camHeight);
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    post.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    if (flow.getWidth() > 0) {
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(flow.getWidth()), SCREEN_HEIGHT / float(flow.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        int controlMax = powf(app->oscControl->controlVal[4] * 0.125, 1.5);
        
        for (int i = 0; i < controlMax; i++) {
            int x = ofRandom(flow.getWidth()-skip);
            int y = ofRandom(flow.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = flow.getAverageFlowInRegion(region);
            
            if (average.length() > thresh) {
                average = ofVec2f(0, 0);
            }
            
            if (abs(average.x) + abs(average.y) > 0.5) {
                ofColor col;
                col.setHsb(ofRandom(255), sat * 255, br * 255);
                
                NoteParticle *p = new NoteParticle();
                p->setup(ofVec2f(x + ofRandom(skip), y + ofRandom(skip)), ofVec2f(average.x * accel, average.y * accel), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                p->friction = friction;
                if (abs(p->radius) > skip) {
                    p->radius = skip;
                }
                particles.push_back(p);
                
                while (particles.size() > controlMax) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < particles.size(); i++) {
            ofSetColor(particles[i]->color);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofPushMatrix();
            ofTranslate(particles[i]->position);
            ofRotateZ(particles[i]->rot.z);
            img[particles[i]->imgNum].draw(0, 0, 0, particles[i]->radius * radius, particles[i]->radius * radius);
            ofPopMatrix();
            ofSetRectMode(OF_RECTMODE_CORNER);
        }
        ofDisableBlendMode();
        ofPopMatrix();
    }
    post.end();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpNote::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StCvOpNote.xml");
    }
}

void StCvOpNote::stateExit(){
    gui->setVisible(false);
}