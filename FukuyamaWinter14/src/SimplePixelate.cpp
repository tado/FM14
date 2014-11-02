#include "SimplePixelate.h"
#include "ofApp.h"

void SimplePixelate::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("SIMPLE PIXELATE");
    gui->addSpacer();
    gui->addSlider("RADIUS", 20, 200, 60);
    gui->addSlider("CIRCLE SCALE", 0, 2.0, 1.0);
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("simple_pixelate.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&SimplePixelate::guiEvent);
}

void SimplePixelate::update(){
    pixels = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
}

void SimplePixelate::draw(){
    //((ofApp*)ofGetAppPtr())->blackmagic->draw();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    int camWidth = 1920;
    int camHeight = 1080;
    
    ofxUISlider *r = (ofxUISlider *)gui->getWidget("RADIUS");
    float radius = r->getValue();
    ofxUISlider *cs = (ofxUISlider *)gui->getWidget("CIRCLE SCALE");
    float circleScale = cs->getValue();
    ofxUIIntSlider *m = (ofxUIIntSlider *)gui->getWidget("MIX");
    int mix = m->getValue();
    
    ofSetCircleResolution(32);
    
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight - radius/2.0; j+=radius){
                ofColor col = pixels.getColor(i + radius / 2.0, j + radius / 2.0);
                float r = col.r / 255.0;
                float g = col.g / 255.0;
                float b = col.b / 255.0;
                
                ofPushMatrix();
                ofTranslate(radius/2.0, radius/2.0);
                ofSetColor(255, 0, 0, mix);
                ofCircle(i + radius * 0.5, j, radius * (float)r * circleScale);
                ofSetColor(0, 255, 0, mix);
                ofCircle(i, j, radius * (float)g * circleScale);
                ofSetColor(0, 0, 255, mix);
                ofCircle(i - radius * 0.5, j, radius * (float)g * circleScale);
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
}

void SimplePixelate::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("simple_pixelate.xml");
    }
}

void SimplePixelate::keyPressed(int key){
    if (key == 'g') {
        gui->toggleVisible();
    }
}

string SimplePixelate::getName(){
    return "simple";
}
