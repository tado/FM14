#include "FatfontPixelate.h"

void FatfontPixelate::setup(){
    font.loadFont("Rotunda.otf", 13);
}

void FatfontPixelate::update(){
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
	}
}

void FatfontPixelate::draw(){
    ofBackground(250);
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    float radius = 10;
    ofPushMatrix();
    ofTranslate(0, radius * ratio.y);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofColor col;
                col.r = r;
                col.g = g;
                col.b = b;
                /*
                int hue = col.getHue();
                int sat = col.getSaturation() * 0.7;
                col.setHsb(hue, sat, 255);
                */
                
                ofSetColor(63);

                int brightness = (r + g + b) / 3;
                int num = ofMap(brightness, 0, 255, 9, 0);
                string str = ofToString(num);
                font.drawString(str, i * ratio.x, j * ratio.y);
            }
        }
    }
    ofPopMatrix();
}

string FatfontPixelate::getName(){
    return "fatfont";
}