#include "OpBlueVector.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpBlueVector::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
}

void OpBlueVector::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void OpBlueVector::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    //img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
    gui.add(skip.setup("Blue skip", 1, 1, 20));
    gui.add(thresh.setup("Blue thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Blue Level", 0, 0, 255));
    gui.add(radius.setup("Blue radius", 0.2, 0.0, 1.0));
    gui.add(accel.setup("Blue accel", 0.12, 0.0, 1.0));
    gui.add(br.setup("Blue brightness", 1.0, 0.0, 1.0));
    gui.add(num.setup("Blue num", 1000, 10, 20000));
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

void OpBlueVector::update() {
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

void OpBlueVector::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(srcLevel, 0, 0);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < 1000; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = farneback.getAverageFlowInRegion(region);
            
            if (average.length() > thresh) {
                average = ofVec2f(0, 0);
            }
            
            if (abs(average.x) + abs(average.y) > 0.5) {
                ofColor col;
                col = ofColor(0, 0, br * 255);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                if (abs(p->radius) > skip) {
                    p->radius = skip;
                }
                particles.push_back(p);
                if (particles.size() > num) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        ofNoFill();
        ofSetLineWidth(3.0);
        ofSetColor(255, 0, 0);
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw();
            //img.draw(particles[i]->position.x, particles[i]->position.y, 2, 2);
        }
        ofSetLineWidth(1.0);
        ofFill();
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpBlueVector::getName(){
    return "opbluevector";
}
