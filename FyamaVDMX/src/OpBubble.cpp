#include "OpBubble.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpBubble::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OpBubble::stateExit(){
    particles.clear();
    deque<Particle*>().swap(particles);
}

void OpBubble::setup() {
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

void OpBubble::update() {
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

void OpBubble::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,255);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()) * 1.1, SCREEN_HEIGHT / float(farneback.getHeight()) * 1.1);
        ofPushMatrix();
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
                col.setHsb(hue, sat * 1.5, br * 2.0);
                
                Particle *p = new Particle();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x / 8.0, average.y / 8.0 - 0.7, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * 0.2;
                if (abs(p->radius) > skip) {
                    p->radius = skip;
                }
                particles.push_back(p);
                if (particles.size() > 10000) {
                    delete particles[0];
                    particles.pop_front();
                }
            }
        }
        
        ofNoFill();
        ofSetLineWidth(4.0);
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw();
        }
        ofSetLineWidth(1.0);
        ofFill();
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpBubble::getName(){
    return "bubble";
}
