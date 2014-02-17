#include "ColorCircle.h"

void ColorCircle::stateEnter(){
    circles.clear();
    ofBackground(0);
}

void ColorCircle::setup(){
    radius = 4;
}

void ColorCircle::update(){
    if (getSharedData().isFrameNew){
        getSharedData().camTexture.readToPixels(pixels);
	}
    
    for(std::vector<MyCircle *>::iterator it = circles.begin(); it != circles.end(); ++it) {
        (*it)->update();
        if ((*it)->live == false) {
            it = circles.erase(it);
        }
    }
}

void ColorCircle::draw(){
    ofVec2f scale;
    int camWidth = getSharedData().camSize.x;
    int camHeight = getSharedData().camSize.y;
    scale.x = ofGetWidth() / float(camWidth);
    scale.y = ofGetHeight() / float(camHeight);
    
    //ofBackground(0);
    
    if (pixels.size()>0){
        for (int i = 0; i < 100; i++) {
            int n = int(ofRandom(camWidth * camHeight)) * 3;
            float x = n % (camWidth * 3) / 3.0 * scale.x;
            float y = n / float(camWidth * 3.0) * scale.x;
            
            float size = 0.1;
            
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            MyCircle *c = new MyCircle(ofColor(r, g, b), ofVec2f(x, y));
            circles.push_back(c);
        }
    }

    for(std::vector<MyCircle *>::iterator it = circles.begin(); it != circles.end(); ++it) {
        (*it)->draw();
    }
}

string ColorCircle::getName(){
    return "circle";
}