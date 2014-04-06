#include "OpSparkle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpSparkle::stateEnter(){
    ofSetColor(0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
}

void OpSparkle::stateExit(){
    particles.clear();
    deque<SparkleParticle*>().swap(particles);
}

void OpSparkle::setup() {
    img.loadImage("sparkle.png");
    
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((testApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((testApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    // GUI
    gui.setup();
    gui.add(skip.setup("Sparkle skip", 1, 1, 20));
    gui.add(thresh.setup("Sparkle thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Sparkle Level", 0, 0, 255));
    gui.add(accel.setup("Sparkle accel", 0.12, 0.0, 1.0));
    gui.add(hue.setup("Notes hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Sparkle saturation", 1.0, 0.0, 1.0));
    gui.add(br.setup("Sparkle brightness", 1.0, 0.0, 1.0));
    gui.add(noteSize.setup("Sparkle size", 1.0, 0.0, 2.0));
    gui.add(num.setup("Sparkle num", 100, 10, 1000));
    gui.add(max.setup("Sparkle max", 100, 10, 1000));
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

void OpSparkle::update() {
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

void OpSparkle::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofSetColor(srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        int currentParticleNum;
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
                col.setHsb(ofRandom(hue * 255 - 10, hue * 255 + 10), sat * 255, br * 255);
                
                SparkleParticle *p = new SparkleParticle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel, 1.0), col);
                p->radius = (abs(average.x) + abs(average.y));

                particles.push_back(p);
                float multi = ofMap(getSharedData().particleNum, 0.0, 1.0, 0.01, 10.0);
                currentParticleNum = num * multi;
                while (particles.size() > currentParticleNum) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        for (int i = 0; i < particles.size(); i++) {
            ofSetColor(particles[i]->color);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofPushMatrix();
            ofTranslate(particles[i]->position);
            ofRotateZ(20);
            img.draw(0, 0, 0, particles[i]->radius * noteSize, particles[i]->radius * noteSize);
            ofPopMatrix();
            ofSetRectMode(OF_RECTMODE_CORNER);
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpSparkle::getName(){
    return "opsparkle";
}
