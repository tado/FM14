#include "OpDistort.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpDistort::stateEnter(){
    verts.clear();
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            verts.push_back(ofVec3f(x * stepSize, y * stepSize, 0));
        }
    }
    flow.resetFlow();
    
    ofEnableArbTex();
}

void OpDistort::setup() {
    gui.setup();
    gui.add(thresh.setup("Distort thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Distort Level", 0, 0, 255));
    gui.add(texLevel.setup("Distort texLevel", 127, 0, 255));
    gui.add(wireLevel.setup("Distort wire Level", 127, 0, 255));
    gui.add(strength.setup("Distort strength", 40.0, 0.0, 200.0));
    gui.add(interpolate.setup("Distort interpolate", 0.1, 0.0, 0.5));
    gui.add(lineWidth.setup("Distort lineWidth", 2.0, 0.0, 5.0));

    gui.loadFromFile("settings.xml");
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 10.0;
    cvWidth = 240;
    cvHeight = 45;
    camWidth = 1920;
    camHeight = 351;
    
    cvScale = ofVec2f(cvWidth/float(camWidth), cvHeight/float(camHeight));
    
    xSteps = camWidth / stepSize;
    ySteps = camHeight / stepSize;
    
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
            mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
            verts.push_back(ofVec3f(x * stepSize, y * stepSize, 0));
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
    float distortionStrength = strength;
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            ofVec2f position(x * stepSize * cvScale.x, y * stepSize * cvScale.y);
            ofRectangle area(position - ofVec2f(stepSize * cvScale.x, stepSize * cvScale.y) / 2,
                             stepSize * cvScale.x, stepSize * cvScale.y);
            ofVec2f offset = flow.getAverageFlowInRegion(area);
            if (offset.length() > thresh){
                offset = ofVec2f(0, 0);
            }
            float zLength = offset.length() * 10;
            if (zLength > 10) {
                zLength = 10;
            }
            ofVec3f offset3 = ofVec3f(offset.x, offset.y, zLength);
            ofVec3f pos3 = ofVec3f(position.x, position.y, 0) / ofVec3f(cvScale.x, cvScale.y, 0);
            ofVec3f newPos = pos3 + distortionStrength * offset3;
            verts[i] += (newPos - verts[i]) * interpolate;
            mesh.setVertex(i, verts[i]);
            i++;
        }
    }
}

void OpDistort::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofSetColor(srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(texLevel);
    ofVec2f scale = ofVec2f(SCREEN_WIDTH / float((xSteps - 1) * stepSize), SCREEN_HEIGHT / float((ySteps - 1) * stepSize));
    ofScale(scale.x, scale.y);

    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.bind();

    ofSetColor(texLevel);
    mesh.draw();
    ofDisableBlendMode();
    tex.unbind();
    
    // set wire color
    ofVec2f average = flow.getAverageFlow();
    float dist = ofMap(average.length(), 0, 2, 0, 127);
    ofColor col;
    col.setHsb(dist, 220, wireLevel);
    ofSetColor(col);
    ofEnableSmoothing();
    ofSetLineWidth(lineWidth);
    ofDisableSmoothing();
    mesh.drawWireframe();
    

    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpDistort::getName(){
    return "opdistort";
}