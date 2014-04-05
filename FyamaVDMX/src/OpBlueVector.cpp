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
    
    img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
    gui.add(sat.setup("Blue sat", 0, 0, 255));
    gui.add(skip.setup("Blue skip", 1, 1, 20));
    gui.add(thresh.setup("Blue thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Blue Level", 0, 0, 255));
    gui.add(radius.setup("Blue radius", 0.2, 0.0, 1.0));
    gui.add(accel.setup("Blue accel", 0.12, 0.0, 2.0));
    //gui.add(br.setup("Blue brightness", 1.0, 0.0, 1.0));
    gui.add(minDist.setup("Blue dist", 10.0, 1.0, 40.0));
    gui.add(num.setup("Blue num", 100, 2, 1000));
    gui.add(max.setup("Blue max", 10, 1, 100));
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
    if (!getSharedData().redBlue) {
        ofSetColor(sat, sat, srcLevel - sat);
    } else {
        ofSetColor(srcLevel - sat, sat, sat);
    }
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
        
        for (int i = 0; i < max; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = farneback.getAverageFlowInRegion(region);
            
            if (average.length() > thresh) {
                average = ofVec2f(0, 0);
            }
            
            if (abs(average.x) + abs(average.y) > 0.5) {
                ofColor col;
                col = ofColor(0, 0, 255);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                if (abs(p->radius) > skip) {
                    p->radius = skip;
                }
                particles.push_back(p);
                while (particles.size() > num) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        ofNoFill();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        for (int i = 0; i < particles.size(); i++) {
            for (int j = 1; j < particles.size()-1; j++) {
                //particles[i]->draw();
                float dist = ofDist(particles[i]->position.x, particles[i]->position.y,
                                    particles[j]->position.x, particles[j]->position.y);
                if(dist < minDist){
                    float level = ofMap(dist, 0, minDist, 255, 0);
                    if (!   getSharedData().redBlue) {
                        ofSetColor(level, 0, 0);
                    } else {
                        ofSetColor(0, 0, level);
                    }
                    ofSetLineWidth(1.5);
                    ofLine(particles[i]->position.x, particles[i]->position.y,
                           particles[j]->position.x, particles[j]->position.y);
                    //ofRect(particles[i]->position.x, particles[i]->position.y, 1.0, 1.0);
                    //ofRect(particles[j]->position.x, particles[j]->position.y, 1.0, 1.0);
                    ofSetLineWidth(1.0);
                }
            }
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
        //ofSetLineWidth(1.0);
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
