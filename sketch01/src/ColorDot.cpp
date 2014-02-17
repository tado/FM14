#include "ColorDot.h"

void ColorDot::setup(){
    radius = 4;
}

void ColorDot::update(){
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
	}
}

void ColorDot::draw(){
    ofVec2f scale;
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    scale.x = ofGetWidth() / float(camWidth);
    scale.y = ofGetHeight() / float(camHeight);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 3);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofPushMatrix();
    ofTranslate(radius / 2.0 * scale.x, radius / 2.0 * scale.y);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    if (pixels.size()>0){
        for (int i = 0; i < 100; i++) {
            int n = int(ofRandom(camWidth * camHeight)) * 3;
            float x = n % (camWidth * 3) / 3.0 * scale.x;
            float y = n / float(camWidth * 3.0) * scale.x;
            
            float size = 0.1;
            
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            /*
            int br = (r + g + b) / 3;
            ofSetColor(r, g, b);

            ofPushMatrix();
            ofCircle(x, y, br * size);
            ofPopMatrix();
             */
            
            int coin = ofRandom(3);
            x += ofRandom(-10, 10);
            y += ofRandom(-10, 10);
            switch (coin) {
                case 0:
                    ofSetColor(255, 0, 0);
                    ofCircle(x, y, r * size);
                    break;
                case 1:
                    ofSetColor(0, 255, 0);
                    ofCircle(x, y, g * size);
                    break;
                case 2:
                    ofSetColor(0, 0, 255);
                    ofCircle(x, y, b * size);
                    break;
                    
                default:
                    break;
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
}

string ColorDot::getName(){
    return "dot";
}