#include "OpVector.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpVector::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
}

void OpVector::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void OpVector::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    // GUI
    gui.setup();
    gui.add(skip.setup("Vector skip", 1, 1, 20));
    gui.add(thresh.setup("Vector thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Vector Level", 0, 0, 255));
    gui.add(radius.setup("Vector radius", 0.2, 0.0, 1.0));
    gui.add(accel.setup("Vector accel", 0.12, 0.0, 1.0));
    gui.add(hue.setup("Vector hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Vector saturation", 1.0, 0.0, 5.0));
    gui.add(br.setup("Vector brightness", 1.0, 0.0, 1.0));
    gui.add(num.setup("Vector num", 1000, 10, 20000));
    gui.add(max.setup("Vector max", 100, 1, 1000));
    gui.loadFromFile("settings.xml");
    
    //CV params
    pyrScale = 0.5;
    levels = 4;
    winsize = 8;
    iterations = 2;
    polyN = 7;
    polySigma = 1.5;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
}

void OpVector::update() {
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void OpVector::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofSetColor(srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < max; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = farneback.getAverageFlowInRegion(region);
            
            if (average.length() > thresh) {
                average = ofVec2f(0, 0);
            }
            
            if (abs(average.x) + abs(average.y) > 0.5) {
                
                int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                unsigned char r = pixels[n];
                unsigned char g = pixels[n + 1];
                unsigned char b = pixels[n + 2];
                
                ofColor col = ofColor(r, g, b);
                int h = col.getHue();
                int s = col.getSaturation();
                int v = col.getBrightness();
                col.setHsb(h * hue, s * sat, v * br);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                //if (abs(p->radius) > skip) {
                //    p->radius = skip;
                //}
                particles.push_back(p);
                if (particles.size() > num) {
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
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpVector::getName(){
    return "opvector";
}
