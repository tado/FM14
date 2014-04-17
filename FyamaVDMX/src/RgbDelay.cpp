#include "RgbDelay.h"
#include "testApp.h"

void RgbDelay::setup(){
    gui.setup();
    gui.add(bufLength.setup("buf Length", 10, 2, 100));
    gui.loadFromFile("settings.xml");
}

void RgbDelay::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    ofTexture t;
    t.loadData(pixels);
    texBuffer.push_back(t);
    while (texBuffer.size() > bufLength) {
        texBuffer.pop_front();
    }
}

void RgbDelay::draw(){
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    if (texBuffer.size() > 1) {
        ofSetColor(255, 0, 0);
        texBuffer[0].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        ofSetColor(0, 255, 0);
        texBuffer[texBuffer.size() / 2].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        ofSetColor(0, 0, 255);
        texBuffer[texBuffer.size() - 1].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    } else {
        ofSetColor(255);
        tex.loadData(pixels);
        tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string RgbDelay::getName(){
    return "rgbdelay";
}
