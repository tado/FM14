#include "OpRadial.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpRadial::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void OpRadial::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void OpRadial::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    // GUI
    gui.setup();
    gui.add(pyrScale.setup("pyrScale", .5, 0, 1));
    gui.add(levels.setup("levels", 4, 1, 8));
    gui.add(winsize.setup("winsize", 8, 4, 64));
    gui.add(iterations.setup("iterations", 2, 1, 8));
    gui.add(polyN.setup("polyN", 7, 5, 10));
    gui.add(polySigma.setup("polySigma", 1.5, 1.1, 2));
    gui.add(OPTFLOW_FARNEBACK_GAUSSIAN.setup("OPTFLOW_FARNEBACK_GAUSSIAN", false));
}

void OpRadial::update() {
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    
    //((testApp*)ofGetAppPtr())->syphonIO.texture.readToPixels(pixels);
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void OpRadial::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,7);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofSetCircleResolution(8);
    //int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    //int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        ofVec2f scale = ofVec2f(ofGetWidth()/float(farneback.getWidth()) * 1.1, ofGetHeight()/float(farneback.getHeight()) * 1.1);
        ofPushMatrix();
        //ofTranslate(ofGetWidth()/3, 0);
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < 1000; i++) {
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
                p->friction = -0.05;
                if (abs(p->radius) > skip / 4.0) {
                    p->radius = skip / 4.0;
                }
                particles.push_back(p);
                if (particles.size() > 10000) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
            /*
             int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
             unsigned char r = pixels[n];
             unsigned char g = pixels[n + 1];
             unsigned char b = pixels[n + 2];
             ofSetColor(r, g, b);
             */
        }
        
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw();
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpRadial::getName(){
    return "opradial";
}
