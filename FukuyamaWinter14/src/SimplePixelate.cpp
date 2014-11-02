#include "SimplePixelate.h"
#include "ofApp.h"

void SimplePixelate::setup(){
    gui = new ofxUICanvas();
    gui->init(201, 0, 200, 200);
    gui->addSpacer();
    gui->addLabel("SIMPLE PIXELATE");
    gui->addSpacer();
    gui->addSlider("RADIUS", 20, 200, 60);
    gui->addSlider("CIRCLE SCALE", 0, 12.0, 2.0);
    gui->addSlider("HUE", 0, 5.0, 1.0);
    gui->addSlider("SAT", 0, 3.0, 1.0);
    gui->addSlider("BR", 0, 1.0, 0.5);
    gui->autoSizeToFitWidgets();

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
    ofxUISlider *hu = (ofxUISlider *)gui->getWidget("HUE");
    float hue = hu->getValue();
    ofxUISlider *sa = (ofxUISlider *)gui->getWidget("SAT");
    float sat = sa->getValue();
    ofxUISlider *b = (ofxUISlider *)gui->getWidget("BR");
    float br = b->getValue();
    
    ofSetCircleResolution(32);
    
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight - radius/2.0; j+=radius){
                ofColor col = pixels.getColor(i + radius / 2.0, j + radius / 2.0);
                int h = col.getHue();
                int s = col.getSaturation();
                int v = col.getBrightness();
                col.setHsb(h * hue, s * sat, v * br);
                
                float r = col.r / 255.0;
                float g = col.g / 255.0;
                float b = col.b / 255.0;
                
                ofPushMatrix();
                ofTranslate(radius/2.0, radius/2.0);
                ofSetColor(255, 0, 0, br * 255);
                ofCircle(i + radius * 0.5, j, radius * (float)r * circleScale);
                ofSetColor(0, 255, 0, br * 255);
                ofCircle(i, j, radius * (float)g * circleScale);
                ofSetColor(0, 0, 255, br * 255);
                ofCircle(i - radius * 0.5, j, radius * (float)g * circleScale);
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
}

void SimplePixelate::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "RADIUS"){

    }
}

string SimplePixelate::getName(){
    return "simple";
}
