#include "OpDistort.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpDistort::stateEnter(){
    
}

void OpDistort::setup() {
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 5.0;
    
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
            verts.push_back(ofVec2f(x * stepSize, y * stepSize));
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
    float distortionStrength = 200.0;
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            ofVec2f position(x * stepSize * cvScale.x, y * stepSize * cvScale.y);
            ofRectangle area(position - ofVec2f(stepSize * cvScale.x, stepSize * cvScale.y) / 2,
                             stepSize * cvScale.x, stepSize * cvScale.y);
            ofVec2f offset = flow.getAverageFlowInRegion(area);
            ofVec2f newPos = position / cvScale + distortionStrength * offset;
            verts[i] += (newPos - verts[i]) * 0.1;
            mesh.setVertex(i, verts[i]);
            i++;
        }
    }
}

void OpDistort::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 127);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(127);
    ofVec2f scale = ofVec2f(ofGetWidth()/float((xSteps - 1) * stepSize), ofGetHeight()/float((ySteps - 1) * stepSize));
    ofScale(scale.x, scale.y);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.bind();
    mesh.draw();
    mesh.drawWireframe();
    tex.unbind();

    ofDisableBlendMode();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpDistort::getName(){
    return "opdistort";
}