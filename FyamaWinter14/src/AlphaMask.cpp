#include "AlphaMask.h"
#include "ofApp.h"

AlphaMask::AlphaMask(){
    ofEnableAlphaBlending();
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
    
    mask.loadImage("mask.png");
    maskShader.load("composite");
    maskShader.begin();
    
    maskShader.setUniformTexture("Tex1", mask.getTextureReference(), 1);
    maskShader.end();
    
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;
}

void AlphaMask::update(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;
    maskShader.setUniformTexture("Tex0", srcTex, 0);
}

void AlphaMask::draw(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;

    fbo.begin();
    maskShader.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    //topLayer.getTextureReference().bind();
    srcTex.bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    mask.getTextureReference().bind();
    
    glBegin(GL_QUADS);
    
    float maskOffset = 0;
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskOffset);
    glVertex2f( 0, 0);
    
    // glMultiTexCoord2d(GL_TEXTURE0_ARB, topLayer.getWidth(), 0);
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), 0);
    glVertex2f( ofGetWidth(), 0);
    
    // glMultiTexCoord2d(GL_TEXTURE0_ARB, topLayer.getWidth(), topLayer.getHeight());
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), srcTex.getHeight());
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), mask.getHeight());
    glVertex2f( ofGetWidth(), ofGetHeight());
    
    // glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, topLayer.getHeight());
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, srcTex.getHeight());
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, mask.getHeight());
    glVertex2f(0, ofGetHeight());
    
    glEnd();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    mask.getTextureReference().unbind();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    srcTex.unbind();
    
    maskShader.end();
    fbo.end();
    
    ofSetColor(255, app->oscControl->controlVal[4] * 2);
    fbo.draw(0, 0);
    
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPushMatrix();
    ofRect(0, 0, 1920, 224);
    ofTranslate(0, 630 + 224);
    ofRect(0, 0, 1920, 224);
    ofPopMatrix();

}