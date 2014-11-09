#include "DrawFbo.h"

DrawFbo::DrawFbo(){
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    post.createPass<GodRaysPass>()->setEnabled(true);
    
    top = 226;
    width = ofGetWidth();
    height = 630;
    fbo.allocate(1920, height);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
}

void DrawFbo::draw(){
    ofSetColor(255);
    post.begin();
    ofTranslate(width, height);
    ofRotateZ(180);
    ofRotateY(180);
    fbo.draw(-width, -top);
    post.end();
}