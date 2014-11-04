#include "BlackmagicCapture.h"

BlackmagicCapture::BlackmagicCapture(int _width, int _height, float _framerate){
    useBlackmagic = true;

    width = _width;
    height = _height;
    framerate = _framerate;
    
    if (useBlackmagic) {
        blackmagic.setup(width, height, framerate);
    } else {
        grabber.initGrabber(width, height);
    }

    colorTexture.allocate(width, height, GL_RGB);
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
}

void BlackmagicCapture::exit() {
    if (useBlackmagic) {
        blackmagic.close();
    }
}

void BlackmagicCapture::update() {
    if (useBlackmagic) {
        if(blackmagic.update()) {
            colorTexture = blackmagic.getColorTexture();
            colorPixels = blackmagic.getColorPixels();
        }
    } else {
        grabber.update();
        if (grabber.isFrameNew()) {
            colorTexture = grabber.getTextureReference();
            colorPixels = grabber.getPixelsRef();
        }
    }

    fbo.begin();
    ofSetColor(255);
    colorTexture.draw(0, 0, width, height/2.0);
    fbo.end();
}

void BlackmagicCapture::draw() {
    ofPushMatrix();
    ofScale(1.0, 2.0);
    fbo.draw(0, 0);
    ofPopMatrix();
}

void BlackmagicCapture::changeInput(bool usebm){
    if (usebm) {
        blackmagic.setup(width, height, framerate);
        useBlackmagic = true;
        grabber.close();
    } else {
        grabber.initGrabber(width, height);
        useBlackmagic = false;
        blackmagic.close();
    }
}