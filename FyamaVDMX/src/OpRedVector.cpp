#include "OpRedVector.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpRedVector::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
}

void OpRedVector::stateExit(){
    particles.clear();
    deque<RectParticle*>().swap(particles);
}

void OpRedVector::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    //img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
    gui.add(sat.setup("Red sat", 0, 0, 255));
    gui.add(skip.setup("Red skip", 1, 1, 20));
    gui.add(thresh.setup("Red thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Red Level", 0, 0, 255));
    gui.add(radius.setup("Red radius", 0.2, 0.0, 10.0));
    gui.add(accel.setup("Red accel", 0.12, 0.0, 1.0));
    gui.add(lfoAmp.setup("Red lfoAmp", 100.0, 0.0, 1000.0));
    gui.add(lfoFreq.setup("Red lfoFreq", 0.05, 0.0, 0.2));
    gui.add(minDist.setup("Red dist", 10.0, 1.0, 40.0));
    gui.add(num.setup("Red num", 500, 10, 1000));
    gui.add(max.setup("Red max", 10, 1, 100));
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

void OpRedVector::update() {
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
        particles[i]->lfoFreq = lfoFreq;
        particles[i]->lfoAmp = lfoAmp;
        particles[i]->update();
    }
}

void OpRedVector::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    if (getSharedData().redBlue) {
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
        int skip = 10;
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
                RectParticle *p = new RectParticle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0.0), ofVec3f(average.x * accel, average.y * accel, 0), ofColor(255));
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                if (abs(p->radius) > skip) {
                    p->radius = skip;
                }
                particles.push_back(p);

                while(particles.size() > num){
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        ofNoFill();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        //ofDisableBlendMode();
        for (int i = 0; i < particles.size(); i++) {
            if (getSharedData().redBlue) {
                ofSetColor(255, 0, 0);
            } else {
                ofSetColor(0, 0, 255);
            }
            ofSetLineWidth(3.0);
            ofRect(particles[i]->position.x, particles[i]->position.y, particles[i]->position.z,
                   particles[i]->radius, particles[i]->radius);
            
            ofSetLineWidth(2.0);
            for (int j = 1; j < particles.size()-1; j++) {
                float dist = sqrt(double((particles[i]->position.x - particles[j]->position.x)
                                         * (particles[i]->position.x - particles[j]->position.x)
                                         + (particles[i]->position.y - particles[j]->position.y)
                                         * (particles[i]->position.y - particles[j]->position.y)
                                         + (particles[i]->position.z - particles[j]->position.z)
                                         * (particles[i]->position.z - particles[j]->position.z)
                                         ));
                if(dist < minDist){
                    float level = ofMap(dist, 0, minDist, 255, 0);
                    
                    
                    if (getSharedData().redBlue) {
                        ofSetColor(level, 0, 0);
                    } else {
                        ofSetColor(0, 0, level);
                    }
                    
                    ofLine(particles[i]->position.x, particles[i]->position.y, particles[i]->position.z,
                           particles[j]->position.x, particles[j]->position.y, particles[j]->position.z);
                }
            }
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
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

void OpRedVector::toggleRedBlue(){
    if (getSharedData().redBlue) {
        getSharedData().redBlue == false;
    } else {
        getSharedData().redBlue == true;
    }
}

string OpRedVector::getName(){
    return "opredvector";
}
