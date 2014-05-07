#include "PhaseShift.h"
#include "testApp.h"

void PhaseShift::setup(){
    gui.setup();
    gui.add(throughLevel.setup("phaseshift through level", 127, 0, 255));
    //gui.add(bufLength.setup("rgb delay Length", 10, 2, 100));
    gui.add(sliceHeight.setup("phaseshift height", 10, 1, 40));
    gui.loadFromFile("settings.xml");
    baseColor.setHsb(ofRandom(255), 255, 255);
    readPix.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);
}

void PhaseShift::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    
    bufLength = SCREEN_HEIGHT / sliceHeight / 2 + 1;
    
    ofTexture t;
    t.loadData(pixels);
    texBuffer.push_back(t);
    while (texBuffer.size() > bufLength) {
        texBuffer.pop_front();
    }
    
    // change color
    if(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor){
        int hue = baseColor.getHue();
        hue = (hue + 80) % 255;
        baseColor.setHsb(hue, 255, 255);
        ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
    }
}

void PhaseShift::draw(){
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofClear(0);
    ofDisableBlendMode();
    
    float height = sliceHeight * 2.0;
    for (int i = 0; i < texBuffer.size() - 1; i++) {
        ofColor col;
        col.setHsb(int(baseColor.getHue() + i * sliceHeight / 10.0)%255, 255, 255);
        ofSetColor(col);
        texBuffer[texBuffer.size() - i - 1].readToPixels(readPix);
        readPix.crop(0, sliceHeight * i, SCREEN_WIDTH, sliceHeight + 1);
        drawTex.loadData(readPix);
        drawTex.draw(0, height * i, SCREEN_WIDTH, height + 1);
    }

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(throughLevel);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string PhaseShift::getName(){
    return "phaseshift";
}
