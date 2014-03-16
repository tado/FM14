#include "Tile.h"
#include "testApp.h"

void Tile::setup(){
    getSharedData().tileDiv = 3;
}

void Tile::update(){

}

void Tile::draw(){
    ofBackground(0);
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < getSharedData().tileDiv; y++) {
            for (int x = 0; x < getSharedData().tileDiv; x++) {
                float width = ofGetWidth() / float(getSharedData().tileDiv) / 3.0;
                float height = ofGetHeight() / float(getSharedData().tileDiv);
                ((testApp*)ofGetAppPtr())->syphonIO.texture.draw(x * width + i * ofGetWidth() / 3.0, y * height, width, height);
            }
        }
    }
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
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