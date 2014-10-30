#include "MoveTile.h"
#include "ofApp.h"

void MoveTile::setup(){
    getSharedData().tileDiv = 2;
    tilePos = 0;
}

void MoveTile::update(){
    
    width = SCREEN_WIDTH / float(getSharedData().tileDiv) / 3.0;
    height = SCREEN_HEIGHT / float(getSharedData().tileDiv);
    
    tilePos--;
    if (tilePos < -height) {
        tilePos += height;
    }
}

void MoveTile::draw(){
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(0, tilePos);
    
    for (int y = 0; y < getSharedData().tileDiv + 1; y++) {
        ((ofApp*)ofGetAppPtr())->syphonIO.texture.draw(SCREEN_WIDTH / 2, y * height, width, height);
    }
    
    ofPopMatrix();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((ofApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

void MoveTile::keyPressed(int key){
    if (key == OF_KEY_UP) {
        getSharedData().tileDiv++;
    }
    if (key == OF_KEY_DOWN) {
        if (getSharedData().tileDiv > 1) {
            getSharedData().tileDiv--;
        }
    }
}

string MoveTile::getName(){
    return "movetile";
}