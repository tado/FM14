#include "MoveTile.h"
#include "testApp.h"

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
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(0, tilePos);
    
    for (int y = 0; y < getSharedData().tileDiv + 1; y++) {
        ((testApp*)ofGetAppPtr())->syphonIO.texture.draw(SCREEN_WIDTH / 2, y * height, width, height);
    }
    
    ofPopMatrix();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
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