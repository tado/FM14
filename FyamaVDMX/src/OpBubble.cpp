#include "OpBubble.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpBubble::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    farneback.resetFlow();
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
    gui.add(skip.setup("Bubble skip", 1, 1, 20));
    gui.add(thresh.setup("Bubble thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Bubble Level", 0, 0, 255));
    gui.add(radius.setup("Bubble radius", 0.2, 0.0, 1.0));
    gui.add(accel.setup("Bubble accel", 0.12, 0.0, 1.0));
    gui.add(speed.setup("Bubble speed", 0.2, 0.0, 1.0));
    gui.add(hue.setup("Bubble hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Bubble saturation", 1.0, 0.0, 5.0));
    gui.add(br.setup("Bubble brightness", 1.0, 0.0, 1.0));
    gui.add(num.setup("Bubble num", 1000, 10, 20000));
    gui.add(max.setup("Bubble max", 10, 1, 200));
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

void OpBubble::update() {
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

void OpBubble::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofSetColor(srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        //int skip = 1;
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        ofTranslate(-skip, 0);
        
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
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0), ofVec3f(average.x * accel, average.y * accel - speed, 0), col);
                p->radius = (abs(average.x) + abs(average.y)) * radius;
                particles.push_back(p);
                if (particles.size() > num) {
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
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpBubble::getName(){
    return "bubble";
}
