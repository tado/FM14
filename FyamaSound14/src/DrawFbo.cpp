#include "DrawFbo.h"
#include "ofApp.h"

DrawFbo::DrawFbo(){

    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    post.setFlip(true);
    
    top = 226;
    width = ofGetWidth();
    height = 630;
    //fbo.allocate(1920, height);
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
}

void DrawFbo::draw(){
    post.begin();
    fbo.draw(0, top);
    post.end();
}