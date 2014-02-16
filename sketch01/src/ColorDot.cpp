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
    ofSetColor(0,3);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofPushMatrix();
    ofTranslate(radius / 2.0 * scale.x, radius / 2.0 * scale.y);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0){
        /*
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofSetColor(255, 0, 0, 180);
                ofCircle((i - radius / 2.0) * scale.x, j * scale.y, radius*(float)r/255.0 * scale.x * 0.7);
                ofSetColor(0, 255, 0, 180);
                ofCircle(i * scale.x, j * scale.y, radius*(float)g/255.0 * scale.x * 0.7);
                ofSetColor(0, 0, 255, 180);
                ofCircle((i + radius / 2.0) * scale.x, j * scale.y, radius*(float)b/255.0 * scale.x * 0.7);
            }
        }
         */
        for (int i = 0; i < 100; i++) {
            int n = int(ofRandom(camWidth * camHeight)) * 3;
            float x = n % (camWidth * 3) / 3.0 * scale.x;
            float y = n / float(camWidth * 3.0) * scale.x;
            
            float size = 0.1;
            
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            int coin = ofRandom(3);
            
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