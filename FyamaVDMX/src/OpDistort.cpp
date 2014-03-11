#include "OpDistort.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpDistort::stateEnter(){
    
}

void OpDistort::setup() {
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 2;
    
    cvWidth = 240;
    cvHeight = 45;
    //cvWidth = 160;
    //cvHeight = 30;
    
    
    ySteps = cvHeight / stepSize;
    xSteps = cvWidth / stepSize;
    
    img.allocate(cvWidth, cvHeight, OF_IMAGE_COLOR); // ofImage instance
    
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
            mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
        }
    }
    for(int y = 0; y + 1 < ySteps; y++) {
        for(int x = 0; x + 1 < xSteps; x++) {
            int nw = y * xSteps + x;
            int ne = nw + 1;
            int sw = nw + xSteps;
            int se = sw + 1;
            mesh.addIndex(nw);
            mesh.addIndex(ne);
            mesh.addIndex(se);
            mesh.addIndex(nw);
            mesh.addIndex(se);
            mesh.addIndex(sw);
        }
    }
}

void OpDistort::update() {
    texPixels = pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    flow.setWindowSize(8);
    flow.calcOpticalFlow(pixels);
    
    ofVec2f scale = ofVec2f(cvWidth / float(ofGetWidth()), cvHeight / float(ofGetHeight()));

    int i = 0;
    float distortionStrength = 4;
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            ofVec2f position(x * stepSize, y * stepSize);
            ofRectangle area(position - ofVec2f(stepSize, stepSize) / 2, stepSize, stepSize);
            ofVec2f offset = flow.getAverageFlowInRegion(area);
            mesh.setVertex(i, position + distortionStrength * offset);
            i++;
        }
    }
}

void OpDistort::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 48);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofDisableSmoothing();
    ofVec2f scale = ofVec2f(ofGetWidth()/float(cvWidth-1), ofGetHeight()/float(cvHeight-1));
    ofPushMatrix();
    ofScale(scale.x, scale.y);
    ofSetLineWidth(2);
	//tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.loadData(pixels);
    tex.bind();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofSetColor(50);
    mesh.drawWireframe();
    ofSetColor(100);
    mesh.draw();
    tex.unbind();
    ofSetLineWidth(1);

    /*
    ofSetColor(15);
    mesh.drawWireframe();
    ofPopMatrix();
     */
    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpDistort::getName(){
    return "opdistort";
}