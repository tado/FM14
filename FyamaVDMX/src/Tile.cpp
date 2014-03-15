#include "Tile.h"
#include "testApp.h"

void Tile::setup(){
    div = 3;
}

void Tile::update(){
    //pixels = ((testApp*)ofGetAppPtr())->syphonIO.pixels;
}

void Tile::draw(){
    ofBackground(0);
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < div; y++) {
            for (int x = 0; x < div; x++) {
                float width = ofGetWidth() / float(div) / 3.0;
                float height = ofGetHeight() / float(div);
                ((testApp*)ofGetAppPtr())->syphonIO.texture.draw(x * width + i * ofGetWidth() / 3.0, y * height, width, height);
            }
        }
    }
    
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

void Tile::keyPressed(int key){
    if (key == OF_KEY_UP) {
        div++;
    }
    if (key == OF_KEY_DOWN) {
        if (div > 1) {
            div--;
        }
    }
}

string Tile::getName(){
    return "tile";
}