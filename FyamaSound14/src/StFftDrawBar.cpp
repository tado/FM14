#include "StFftDrawBar.h"
#include "ofApp.h"

string StFftDrawBar::getName(){
    return "StFftDrawBar";
}

void StFftDrawBar::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addIntSlider("PLOT HEIGHT", 0, ofGetHeight(), 500);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addIntSlider("BRIGHTNESS", 0, 255, 100);
    gui->addIntSlider("REPEAT", 1, 10, 6);
    gui->addSlider("ZOOM", 1.0, 5.0, 1.2);
    gui->addSlider("LINE WIDTH", 0.1, 10.0, 2.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDrawBar.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDrawBar::guiEvent);
    app = ((ofApp*)ofGetAppPtr());
}

void StFftDrawBar::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDrawBar::draw() {
    ofxUIIntSlider *gplotsize = (ofxUIIntSlider *)gui->getWidget("PLOT HEIGHT"); int plotsize = gplotsize->getValue();

    app->drawFbo->fbo.begin();
    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0);
    plot(app->fft->drawBins, -plotsize, plotsize / 2);
    cam.end();
    app->drawFbo->fbo.end();
}

void StFftDrawBar::plot(vector<float>& buffer, float scale, float offset) {
    ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbr = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbr->getValue();
    ofxUIIntSlider *grep = (ofxUIIntSlider *)gui->getWidget("REPEAT"); int rep = grep->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *glinewidth = (ofxUISlider *)gui->getWidget("LINE WIDTH"); float linewidth = glinewidth->getValue();
    
    int n = buffer.size();
    ofPushMatrix();
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofScale(ofGetWidth() / float(app->fft->drawBins.size()) * zoom , ofGetWidth() / float(app->fft->drawBins.size()) * zoom);
    ofColor col;
    ofSetLineWidth(linewidth * zoom);
    for (int j = 0; j < rep; j++) {
        for (int i = 0; i < n; i++) {
            float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
            int br = ofMap(buffer[i], 0, 1.0, 0, 255 / float(rep)) * brightness / 64.0;
            col.setHsb(hue, saturation, br);
            ofSetColor(col);
            //ofLine(i / 2.0, -scale / 2.0, i / 2.0, scale / 2.0);
            //ofLine(-i / 2.0, -scale / 2.0, -i / 2.0, scale / 2.0);
            ofRect(i / 2.0, -scale / 2.0, linewidth * zoom, scale);
            ofRect(-i / 2.0, -scale / 2.0, linewidth * zoom, scale);

        }
        ofRotateZ(180 / rep);
    }
    ofSetLineWidth(1.0);
    glPopMatrix();
    ofDisableAlphaBlending();
}

void StFftDrawBar::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDrawBar.xml");
    }
}

void StFftDrawBar::stateExit(){
    gui->setVisible(false);
}
