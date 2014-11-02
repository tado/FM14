#include "BlackmagicCapture.h"

BlackmagicCapture::BlackmagicCapture(int _width, int _height, float _framerate){
    width = _width;
    height = _height;
    framerate = _framerate;
    cam.setup(width, height, framerate);
    colorTexture.allocate(width, height, GL_RGB);

    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
}

void BlackmagicCapture::exit() {
    cam.close();
}

void BlackmagicCapture::update() {
    if(cam.update()) {
        colorTexture = cam.getColorTexture();
        colorPixels = cam.getColorPixels();
        fbo.begin();
        ofSetColor(255);
        colorTexture.draw(0, 0, width, height/2.0);
        fbo.end();
    }
}

void BlackmagicCapture::draw() {
    ofPushMatrix();
    ofScale(1.0, 2.0);
    fbo.draw(0, 0);
    ofPopMatrix();
}