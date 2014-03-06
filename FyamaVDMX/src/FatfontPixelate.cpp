#include "FatfontPixelate.h"
#include "testApp.h"

void FatfontPixelate::setup(){
    font.loadFont("Rotunda.otf", 30, false);
    radius = 7;
}

void FatfontPixelate::update(){
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void FatfontPixelate::draw(){
    ofBackground(0);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);

    ofPushMatrix();
    ofTranslate(0, radius * ratio.y);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofColor col;
                col = ofColor(r, g, b);
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, sat, 255);
                
                ofSetColor(col);

                int brightness = 255 - ((r + g + b) / 3.0);
                int num = ofMap(brightness, 0, 255, 9, 0);
                string str = ofToString(num);
                font.drawString(str, i * ratio.x, j * ratio.y);
            }
        }
    }
    ofPopMatrix();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string FatfontPixelate::getName(){
    return "fatfont";
}