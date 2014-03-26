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
    
    //initial crop Y position
    ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().cropPosY = 0.5;
}

void SyphonIO::update(){
    //update pixels
    texture.readToPixels(pixels);

    //crop pixels to cropPixels
    if (((testApp*)ofGetAppPtr())->stateMachine.getSharedData().threeHead) {
        // three head
        float y = ofMap(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().cropPosY,
                        0, 1, 0, pixels.getHeight() - pixels.getHeight() / 36 * 9);
        //pixels.cropTo(croppedPixels, 0, y, pixels.getWidth(), pixels.getHeight() / 36 * 9);
        pixels.cropTo(croppedPixels, 0, y, pixels.getWidth(), pixels.getHeight() / 3.0);
    } else {
        // center screen only
        ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().cropPosY = 0.5;
        pixels.resize(pixels.getWidth() / 3.0, pixels.getHeight() / 3.0);
        croppedPixels.allocate(pixels.getWidth() * 3, pixels.getHeight(), OF_PIXELS_RGB);
        int i = 0;
        while( i < croppedPixels.size()) {
            croppedPixels[i] = 0;
            i++;
        }
        pixels.pasteInto(croppedPixels, pixels.getWidth(), 0);
    }
    
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