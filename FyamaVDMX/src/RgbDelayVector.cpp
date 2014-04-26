#include "RgbDelayVector.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void RgbDelayVector::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
    
    baseColor.setHsb(ofRandom(255), 255, throughLevel);
}

void RgbDelayVector::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void RgbDelayVector::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
    gui.add(wireLevel.setup("RGB wire Level", 127, 0, 255));
    gui.add(skip.setup("RGB skip", 1, 1, 20));
    gui.add(thresh.setup("RGB thresh", 5, 0, 10));
    gui.add(radius.setup("RGB radius", 0.2, 0.0, 1.0));
    gui.add(accel.setup("RGB accel", 0.12, 0.0, 2.0));
    gui.add(minDist.setup("RGB dist", 10.0, 1.0, 40.0));
    gui.add(num.setup("RGB num", 100, 2, 1000));
    gui.add(max.setup("RGB max", 10, 1, 100));
    
    gui.add(throughLevel.setup("RGB through level", 127, 0, 255));
    gui.add(rgbLevel.setup("RGB delay level", 127, 0, 255));
    gui.add(bufLength.setup("RGB delay Length", 10, 2, 100));
    gui.loadFromFile("settings.xml");
    
    //CV params
    pyrScale = 0.5;
    levels = 4;
    winsize = 8;
    iterations = 2;
    polyN = 7;
    polySigma = 1.5;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
    
    //change color
    ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
}

void RgbDelayVector::update() {
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    
    pixels = ((testApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    ofTexture t;
    t.loadData(pixels);
    texBuffer.push_back(t);
    while (texBuffer.size() > bufLength) {
        texBuffer.pop_front();
    }
    
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
    
    // change color
    if(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor){
        int hue = baseColor.getHue();
        hue = (hue + 80) % 255;
        baseColor.setHsb(hue, 255, throughLevel);
        ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().changeColor = false;
    }
}

void RgbDelayVector::draw() {
    int currentParticleNum;
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    ofBackground(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    // RGB delay
    if (texBuffer.size() > 3) {
        ofSetColor(baseColor);
        texBuffer[texBuffer.size() - 1].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        ofSetColor(rgbLevel, 0, 0);
        texBuffer[0].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        ofSetColor(0, rgbLevel, 0);
        texBuffer[texBuffer.size() / 3].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        ofSetColor(0, 0, rgbLevel);
        texBuffer[texBuffer.size() / 3 * 2].draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    
    // particle
    
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
                col.setHsb(ofRandom(255), 255, 127);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel, 0), col);
                p->radius = 0.2;
                particles.push_back(p);

                float multi = ofMap(getSharedData().particleNum, 0.0, 1.0, 0.0, 10.0);
                currentParticleNum = num * multi;
                while (particles.size() > currentParticleNum) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        // draw wire
        for (int i = 0; i < particles.size(); i++) {
            ofFill();
            particles[i]->draw();
            ofNoFill();
            for (int j = 1; j < particles.size()-1; j++) {
                //particles[i]->draw();
                float dist = ofDist(particles[i]->position.x, particles[i]->position.y,
                                    particles[j]->position.x, particles[j]->position.y);
                if(dist < minDist){
                    float level = ofMap(dist, 0, minDist, 255, 0);
                    ofSetColor(particles[i]->color);
                    ofSetLineWidth(1.5);
                    ofLine(particles[i]->position.x, particles[i]->position.y,
                           particles[j]->position.x, particles[j]->position.y);
                    ofSetLineWidth(1.0);
                }
            }
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

string RgbDelayVector::getName(){
    return "rgbdelay";
}
