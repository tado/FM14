#include "OpDistort.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpDistort::stateEnter(){
    
}

void OpDistort::setup() {
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 8.0;
    
    cvWidth = 240;
    cvHeight = 45;
    
    camWidth = 640;
    camHeight = 117;
    
    cvScale = ofVec2f(cvWidth/float(camWidth), cvHeight/float(camHeight));
    
    xSteps = camWidth / stepSize;
    ySteps = camHeight / stepSize;
    
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
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    flow.setWindowSize(8);
    flow.calcOpticalFlow(pixels);
    
    int i = 0;
    float distortionStrength = 40;
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            ofVec2f position(x * stepSize * cvScale.x, y * stepSize * cvScale.y);
            ofRectangle area(position - ofVec2f(stepSize * cvScale.x, stepSize * cvScale.y) / 2,
                             stepSize * cvScale.x, stepSize * cvScale.y);
            ofVec2f offset = flow.getAverageFlowInRegion(area);
            mesh.setVertex(i, position / cvScale + distortionStrength * offset);
            i++;
        }
    }
}

void OpDistort::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 31);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(63);
    ofVec2f scale = ofVec2f(ofGetWidth()/float(camWidth), ofGetHeight()/float(camHeight));
    ofScale(scale.x, scale.y);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.bind();
    mesh.draw();
    //ofSetColor(63);
    mesh.drawWireframe();
    tex.unbind();

    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpDistort::getName(){
    return "opdistort";
}