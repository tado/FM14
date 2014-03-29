#include "SimplePixelate.h"
#include "testApp.h"

void SimplePixelate::setup(){
    gui.setup();
    gui.add(radius.setup("Simple Radius", 20, 1, 50));
    gui.add(srcLevel.setup("Simple Level", 0, 0, 255));
    gui.add(circleScale.setup("Simple scale", 0.02, 0.0, 0.1));
    gui.loadFromFile("settings.xml");
}

void SimplePixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void SimplePixelate::draw(){
    ofVec2f scale;
    
    int camWidth, camHeight;
    
    camWidth = pixels.getWidth();
    camHeight = pixels.getHeight();

    scale.x = SCREEN_WIDTH / float(camWidth);
    scale.y = SCREEN_HEIGHT / float(camHeight);
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofPushMatrix();
    ofTranslate(0, radius / 2.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i) * 3];
                unsigned char g = pixels[(j * camWidth + i) * 3 + 1];
                unsigned char b = pixels[(j * camWidth + i) * 3 + 2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle(i * scale.x + radius * 1.1, j * scale.y, radius * (float)r * circleScale);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius * (float)g * circleScale);
                ofSetColor(0, 0, 255, 180);
                ofCircle(i * scale.x -  radius * 1.1, j * scale.y, radius * (float)b * circleScale);
                
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string SimplePixelate::getName(){
    return "simple";
}