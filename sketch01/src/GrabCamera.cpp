#include "GrabCamera.h"

void GrabCamera::setup(){
    camSize = ofVec2f(640,480);
    cam.setDesiredFrameRate(60);
    cam.initGrabber(camSize.x, camSize.y);
    camTexture = cam.getTextureReference();
}

void GrabCamera::update(){
    cam.update();
}