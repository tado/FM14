#include "OpRadial.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void OpRadial::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OpRadial::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void OpRadial::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((ofApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((ofApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    // GUI
    gui.setup();
    gui.add(fade.setup("Distort fade", 0.5, 0.0, 1.0));
    gui.add(friction.setup("Distort friction", 0.0, -0.1, 0.1));
    gui.add(num.setup("Distort number", 10000, 0, 20000));
    gui.add(max.setup("Distort max", 100, 1, 1000));
    
    //CV params
    pyrScale = 0.5;
    levels = 4;
    winsize = 8;
    iterations = 2;
    polyN = 7;
    polySigma = 1.5;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
}

void OpRadial::update() {
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void OpRadial::draw() {
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,fade * 32);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()) * 1.1, SCREEN_HEIGHT / float(farneback.getHeight()) * 1.1);
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < max; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = farneback.getAverageFlowInRegion(region);
            if (abs(average.x) + abs(average.y) > 0.5) {
                
                int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                unsigned char r = pixels[n];
                unsigned char g = pixels[n + 1];
                unsigned char b = pixels[n + 2];
                
                ofColor col = ofColor(r, g, b);
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, 0, 255);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * 1.0, average.y * 1.0, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * 0.05 + 0.2;
                p->friction = friction;
                if (abs(p->radius) > skip / 4.0) {
                    p->radius = skip / 4.0;
                }
                particles.push_back(p);
                while (particles.size() > num) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw();
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpRadial::getName(){
    return "opradial";
}
