#include "Tile.h"
#include "testApp.h"

void Tile::setup(){
    getSharedData().tileDiv = 3;
}

void Tile::update(){
    
}

void Tile::draw(){
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofBackground(0);
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < getSharedData().tileDiv; y++) {
            for (int x = 0; x < getSharedData().tileDiv; x++) {
                float width = SCREEN_WIDTH / float(getSharedData().tileDiv) / 3.0;
                float height = SCREEN_HEIGHT / float(getSharedData().tileDiv);
                ((testApp*)ofGetAppPtr())->syphonIO.texture.draw(x * width + i * SCREEN_WIDTH / 3.0, y * height, width, height);
            }
        }
    }
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
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