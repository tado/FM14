#include "SimplePixelate.h"

void SimplePixelate::setup(){
    radius = 4;
}

void SimplePixelate::update(){
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
	}
}

void SimplePixelate::draw(){
    ofBackground(0);
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    ofVec2f ratio;
    ratio.x = ofGetWidth() / float(camWidth);
    ratio.y = ofGetHeight() / float(camHeight);
    ofPushMatrix();
    ofTranslate(radius / 2.0 * ratio.x, radius / 2.0 * ratio.y);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    {
        if (pixels.size()>0){
            for (int i = 0; i < camWidth; i+=radius){
                for (int j = 0; j < camHeight; j+=radius){
                    unsigned char r = pixels[(j * camWidth + i)*3];
                    unsigned char g = pixels[(j * camWidth + i)*3+1];
                    unsigned char b = pixels[(j * camWidth + i)*3+2];
                    
                    ofSetColor(255, 0, 0, 180);
                    ofCircle((i - radius / 2.0) * ratio.x, j * ratio.y, radius*(float)r/255.0*ratio.x*0.7);
                    ofSetColor(0, 255, 0, 180);
                    ofCircle(i * ratio.x, j * ratio.y, radius*(float)g/255.0*ratio.x*0.7);
                    ofSetColor(0, 0, 255, 180);
                    ofCircle((i + radius / 2.0) * ratio.x, j * ratio.y, radius*(float)b/255.0*ratio.x*0.7);
                    
                }
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
}

string SimplePixelate::getName(){
    return "simple";
}