#include "StKaleidoscope.h"

string StKaleidoscope::getName(){
    return "StKaleidoscope";
}

void StKaleidoscope::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("KALEIDO SCOPE");
    // gui->addSlider("HUE", 0, 1.0, 0.5);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StKaleidoscope.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StKaleidoscope::guiEvent);
    
    post.init(ofGetWidth(), ofGetHeight());

    kaleido = post.createPass<KaleidoscopePass>();
    kaleido->setSegments(3.0);
    kaleido->setEnabled(true);
    noise = post.createPass<NoiseWarpPass>();
    noise->setEnabled(true);
    noise->setFrequency(2.0);

    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StKaleidoscope::update(){
    fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    float noiseStrength = ofMap(app->oscControl->controlVal[2], 0, 127, 5, 10);
    noise->setFrequency(fftSum/ float(app->fft->drawBins.size()) * noiseStrength);
    noise->setAmplitude(fftSum/ float(app->fft->drawBins.size()) * noiseStrength);
    //noise->setFrequency(fftSum/ float(app->fft->drawBins.size()) * 20.0);
    //float segment = ofMap(app->oscControl->controlVal[2], 0, 127, 1, 3);
    //kaleido->setSegments(segment);
    //kaleido->setAspect(2000 - ofVec2f(fftSum, fftSum) * 1000);
}

void StKaleidoscope::draw(){
    //ofxUIIntSlider *gmix = (ofxUIIntSlider *)gui->getWidget("MIX"); int mix = gmix->getValue();
    // ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    float hue =ofMap(app->oscControl->controlVal[3], 0, 127, 0.0, 1.0);
    app->drawFbo->fbo.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    post.begin(cam);
    ofPushMatrix();
    ofScale(2.0, 2.0);
    ofColor col; col.setHsb(hue * 255, sat * 255, br * 255);
    ofSetColor(col);
    ofRotateZ(ofGetElapsedTimef() * 40.0);
    app->blackmagic->colorTexture.draw(-app->blackmagic->width/2, -app->blackmagic->height/2, app->blackmagic->width, app->blackmagic->height);
    ofPopMatrix();
    post.end();
    app->drawFbo->fbo.end();
    ofDisableBlendMode();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StKaleidoscope::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StKaleidoscope.xml");
    }
}

void StKaleidoscope::stateExit(){
    gui->setVisible(false);
}
