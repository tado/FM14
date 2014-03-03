#include "SimplePixelate.h"
#include "testApp.h"

void SimplePixelate::setup(){
    radius = 6;
    int width = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int height = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(width, height, 3);
}

void SimplePixelate::update(){
    ((testApp*)ofGetAppPtr())->syphonIO.texture.readToPixels(pixels);
}

void SimplePixelate::draw(){
    ofVec2f scale;
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    scale.x = ofGetWidth() / float(camWidth);
    scale.y = ofGetHeight() / float(camHeight);
    
    ofBackground(0);
    ofPushMatrix();
    ofTranslate(radius / 2.0 * scale.x, -ofGetHeight() + radius / 2.0 * scale.y);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = camHeight/3; j < camHeight/3*2; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle((i - radius / 4.0) * scale.x, j * scale.y * 3.0, radius*(float)r/255.0 * scale.x * 0.7);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y * 3.0, radius*(float)g/255.0 * scale.x * 0.7);
                ofSetColor(0, 0, 255, 180);
                ofCircle((i + radius / 4.0) * scale.x, j * scale.y * 3.0, radius*(float)b/255.0 * scale.x * 0.7);
                
            }
        }
        ofDisableBlendMode();
        ofPopMatrix();
    }
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.gui.draw();
}

string SimplePixelate::getName(){
    return "simple";
}