#include "ColorBubble.h"
#include "testApp.h"

void ColorBubble::setup(){
    baseColor.setHsb(ofRandom(255), 255, 255);
    gui.setup();
    gui.add(baseLevel.setup("Bubble base Level", 127, 0, 255));
    gui.add(bubbleLevel.setup("Bubble Level", 31, 0, 127));
    gui.add(bubbleNum.setup("Bubble num", 200, 1, 400));
    gui.loadFromFile("settings.xml");
    
    for (int i = 0; i < bubbleNum; i++) {
        bubbles.push_back(new FloatingBubble());
    }
}

void ColorBubble::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    
    while (bubbles.size() < bubbleNum) {
        bubbles.push_back(new FloatingBubble());
    }
    
    while (bubbles.size() > bubbleNum) {
        bubbles.pop_back();
    }
    
    for (int i = 0; i < bubbles.size(); i++) {
        bubbles[i]->update();
    }

    // change color
    if(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor){
        for (int i = 0; i < bubbles.size(); i++) {
            ofColor col;
            col.setHsb(ofRandom(255), 255, 255);
            bubbles[i]->color = col;
        }
        int hue = baseColor.getHue();
        hue = (hue + 80) % 255;
        baseColor.setHsb(hue, 255, 255);
        ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
    }
}

void ColorBubble::draw(){

    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofClear(0);

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofColor col;
    col.setHsb(baseColor.getHue(), 255, baseLevel);
    ofSetColor(col);
    tex.loadData(pixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    for (int i = 0; i < bubbles.size(); i++) {
        bubbles[i]->level = bubbleLevel;
        bubbles[i]->draw();
    }
    
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string ColorBubble::getName(){
    return "colorbubble";
}
