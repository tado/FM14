#include "SyphonIO.h"
#include "testApp.h"

void SyphonIO::setup(int _width, int _height){
    width = _width;
    height = _height;
    
    //setup syphon client
    fboSyphonIn.allocate(width, height, GL_RGB);
    texture.allocate(width, height, GL_TEXTURE_2D);
    client.setup();
    
    //setup syphon server
    //fboSyphonOut.allocate(width, height, GL_RGBA);
    fboSyphonOut.allocate(3840, 720, GL_RGBA);
    server.setName("oF");
    
    outTex.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);
    fbo.allocate(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //gui setup
    gui.setup();
    gui.add(fitScreen.setup("Fit to screen", false));
    gui.add(screenPos.setup("Screen position", ofGetWidth()/2, 0, ofGetWidth()));
}

void SyphonIO::update(){
    //update pixels
    texture.readToPixels(pixels);
    
    //crop pixels to cropPixels
    pixels.cropTo(croppedPixels, 0, pixels.getHeight() / 8 * 3, pixels.getWidth(), pixels.getHeight() / 36 * 9);

    //update syphone client
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