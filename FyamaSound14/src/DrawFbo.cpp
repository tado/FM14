#include "DrawFbo.h"

DrawFbo::DrawFbo(){
    top = 226;
    height = 630;
    fbo.allocate(1920, height);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
}

void DrawFbo::draw(){
    ofSetColor(255);
    fbo.draw(0, top);
}