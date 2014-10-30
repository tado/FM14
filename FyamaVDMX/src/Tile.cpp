#include "Tile.h"
#include "ofApp.h"

void Tile::setup(){
    getSharedData().tileDiv = 3;
}

void Tile::update(){
    
}

void Tile::draw(){
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofBackground(0);
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < getSharedData().tileDiv; y++) {
            for (int x = 0; x < getSharedData().tileDiv; x++) {
                float width = SCREEN_WIDTH / float(getSharedData().tileDiv) / 3.0;
                float height = SCREEN_HEIGHT / float(getSharedData().tileDiv);
                ((ofApp*)ofGetAppPtr())->syphonIO.texture.draw(x * width + i * SCREEN_WIDTH / 3.0, y * height, width, height);
            }
        }
    }
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((ofApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

void Tile::keyPressed(int key){
    if (key == OF_KEY_UP) {
        getSharedData().tileDiv++;
    }
    if (key == OF_KEY_DOWN) {
        if (getSharedData().tileDiv > 1) {
            getSharedData().tileDiv--;
        }
    }
}

string Tile::getName(){
    return "tile";
}