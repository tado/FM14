#include "SyphonIO.h"

void SyphonIO::setup(int _width, int _height){
    width = _width;
    height = _height;
    
    //setup syphon client
    fboSyphonIn.allocate(width, height, GL_RGB);
    texture.allocate(width, height, GL_TEXTURE_2D);
    client.setup();
    
    //setup syphon server
    fboSyphonOut.allocate(width, height, GL_RGBA);
    
}

void SyphonIO::update(){
    // update syphone client
    ofSetColor(255);
    fboSyphonIn.begin();
    {
        ofClear(0,0,0,0);
        client.draw(0, 0, width, height);
    }
    fboSyphonIn.end();
    texture = fboSyphonIn.getTextureReference();
}

void SyphonIO::draw(){

}