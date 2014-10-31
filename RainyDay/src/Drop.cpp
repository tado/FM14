#include "Drop.h"
#include "ofApp.h"

Drop::Drop(ofTexture *texture, ofVec2f pos, float rad){
    circleTexture = texture;
    position = pos;
    radius = rad;
}

void Drop::draw(){
    ofSetColor(255, 255, 255);
    
    circleTexture->bind();
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < ((ofApp*)ofGetAppPtr())->NormCirclePts.size(); i++){
        glTexCoord2f(((ofApp*)ofGetAppPtr())->NormCircleCoords[i].x, ((ofApp*)ofGetAppPtr())->NormCircleCoords[i].y);
        glVertex2f( ((ofApp*)ofGetAppPtr())->NormCirclePts[i].x * radius + position.x,  ((ofApp*)ofGetAppPtr())->NormCirclePts[i].y * radius + position.y);
    }
    glEnd();
    
    circleTexture->unbind();
}