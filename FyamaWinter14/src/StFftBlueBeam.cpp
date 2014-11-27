#include "StFftBlueBeam.h"
#include "ofApp.h"

string StFftBlueBeam::getName(){
    return "StFftBlueBeam";
}

void StFftBlueBeam::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT BEAM");
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftBlueBeam::guiEvent);
    app = ((ofApp*)ofGetAppPtr());
    
    //post.init(app->drawFbo->width, app->drawFbo->height);
    //post.createPass<BloomPass>()->setEnabled(true);
    cam.setFarClip(2000);
    
    createMesh();
}

void StFftBlueBeam::update(){
    for (int i = 0; i < NUM; i++) {
        position[i].z += 40.0;
        if (position[i].z > 0) {
            position[i].z -= 10000;
        }
        mesh.setVertex(i, position[i]);
    }
    gui->setVisible(getSharedData().guiVisible);
}

void StFftBlueBeam::draw(){
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    //post.begin(cam);
    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, 8);
    ofRect(-ofGetWidth(), -ofGetHeight(), ofGetWidth()*2, ofGetHeight()*2);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glPointSize(4.0);
    ofSetColor(0, 127, 255);
    mesh.draw();
    ofDisableAlphaBlending();
    cam.end();
    //post.end();
    app->drawFbo->fbo.end();
}

void StFftBlueBeam::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StFftBlueBeam::createMesh(){
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < NUM; i++) {
        ofVec3f pos = ofVec3f(ofRandom(-ofGetWidth(), ofGetWidth()),
                              ofRandom(-ofGetHeight(), ofGetHeight()),
                              ofRandom(-10000, 0));
        position.push_back(pos);
        mesh.addVertex(pos);
    }
        
}

void StFftBlueBeam::stateExit(){
    gui->setVisible(false);
}
