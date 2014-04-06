#include "Through.h"
#include "testApp.h"

void Through::setup(){
}

void Through::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void Through::draw(){
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofSetColor(255);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
}

string Through::getName(){
    return "through";
}
