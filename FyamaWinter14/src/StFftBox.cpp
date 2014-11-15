#include "StFftBox.h"
#include "ofApp.h"

string StFftBox::getName(){
    return "StFftBox";
}

void StFftBox::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Box");
    gui->addSpacer();
    gui->addSlider("BOX SIZE", 0, ofGetHeight(), 50.0);
    gui->addIntSlider("SKIP", 1, 12, 4);
    gui->addSlider("STIFFNESS", 0, 3.0, 1.0);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addIntSlider("BRIGHTNESS", 0, 127, 5);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBox.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftBox::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
    int fftsize = app->bufferSize;
    force = new float[fftsize];
    size = new float[fftsize];
    rotSize = new ofVec3f[fftsize];
    for(int i = 0; i < fftsize; i++){
        force[i] = 0.0;
        size[i] = 0.0;
        rotSize[i] = ofVec3f(ofRandom(-0.2, 0.2), ofRandom(-0.2, 0.2), ofRandom(-0.2, 0.2));
    }
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    //post.setFlip(true);
}

void StFftBox::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftBox::draw() {
    ofxUISlider *gboxsize = (ofxUISlider *)gui->getWidget("BOX SIZE"); float boxsize = gboxsize->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUISlider *gstiffness = (ofxUISlider *)gui->getWidget("STIFFNESS"); float stiffness = gstiffness->getValue();
    ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbrightness = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbrightness->getValue();
    
    app->drawFbo->fbo.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    //cam.begin();
    post.begin(cam);
    ofTranslate(0, app->drawFbo->top / 2.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableDepthTest();
    //glCullFace(true);
    for (int i = 0; i < app->fft->drawBins.size(); i += skip){
        ofPushMatrix();
        float springForce = stiffness * -size[i];
        float addForce = app->fft->drawBins[i] * boxsize;
        force[i] = (springForce + addForce) * 0.2;
        size[i] += force[i];

        float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
        if (hue > 255) {
            hue = 255;
        }
        ofColor col;
        col.setHsb(hue, saturation, brightness);
        ofSetColor(col);
        ofRotateX(size[i] * rotSize[i].x);
        ofRotateY(size[i] * rotSize[i].y);
        ofRotateZ(size[i] * rotSize[i].z);
        ofBoxPrimitive box;
        box.set(size[i]);
        box.setResolution(2);
        box.draw();
        ofPopMatrix();
    }
    //glCullFace(false);
    ofDisableDepthTest();
    ofDisableAlphaBlending();
    //cam.end();
    post.end();
    app->drawFbo->fbo.end();
}

void StFftBox::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBox.xml");
    }
}

void StFftBox::stateExit(){
    gui->setVisible(false);
}