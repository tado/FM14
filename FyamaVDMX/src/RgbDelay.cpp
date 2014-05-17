#include "RgbDelay.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void RgbDelay::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    baseColor.setHsb(ofRandom(255), 255, throughLevel);
}

void RgbDelay::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void RgbDelay::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
        
    gui.add(throughLevel.setup("RGB through level", 127, 0, 255));
    gui.add(rgbLevel.setup("RGB delay level", 127, 0, 255));
    gui.add(bufLength.setup("RGB delay Length", 10, 2, 100));
    gui.add(delayNum.setup("RGB delay num", 4, 1, 12));

    gui.loadFromFile("settings.xml");
    
    //change color
    ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
}

void RgbDelay::update() {
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    ofTexture t;
    t.loadData(pixels);
    texBuffer.push_back(t);
    while (texBuffer.size() > bufLength) {
        texBuffer.pop_front();
    }
    
    pixels.resize(cvWidth, cvHeight);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
    
    // change color
    if(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor){
        int hue = baseColor.getHue();
        hue = (hue + 80) % 255;
        baseColor.setHsb(hue, 255, throughLevel);
        ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
    }
}

void RgbDelay::draw() {
    int currentParticleNum;
    ofColor col;
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofClear(0);
    
    ofSetColor(baseColor,throughLevel);
    texBuffer[texBuffer.size() - 1].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (texBuffer.size() > delayNum) {
        for (int i = 0; i < delayNum; i++) {
            col.setHsb(int(baseColor.getHue()+ 255/delayNum * i) % 255, 255, rgbLevel/delayNum * 2);
            ofSetColor(col);
            texBuffer[texBuffer.size() / delayNum * i].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
    
    // RGB delay
    /*
    if (texBuffer.size() > 6) {
        ofSetColor(baseColor,throughLevel);
        texBuffer[texBuffer.size() - 1].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        col.setHsb(int(baseColor.getHue()+0)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[0].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        col.setHsb(int(baseColor.getHue()+20)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[texBuffer.size() / 6].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        col.setHsb(int(baseColor.getHue()+40)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[texBuffer.size() / 6 * 2].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        col.setHsb(int(baseColor.getHue()+80)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[texBuffer.size() / 6 * 3].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        col.setHsb(int(baseColor.getHue()+120)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[texBuffer.size() / 6 * 4].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        col.setHsb(int(baseColor.getHue()+180)%255, 255, rgbLevel);
        ofSetColor(col);
        texBuffer[texBuffer.size() / 6 * 5].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
     */
    
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string RgbDelay::getName(){
    return "rgbdelay";
}
