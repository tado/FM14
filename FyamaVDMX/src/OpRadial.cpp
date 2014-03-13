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
    ribbons.clear();
    deque<Ribbon*>().swap(ribbons);
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
    
    for (int i = 0; i < ribbons.size(); i++) {
        ribbons[i]->update();
    }
}

void OpRadial::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        ofVec2f scale = ofVec2f(ofGetWidth()/float(farneback.getWidth()) * 1.1, ofGetHeight()/float(farneback.getHeight()) * 1.1);
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        
        for (int i = 0; i < 1000; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f average = farneback.getAverageFlowInRegion(region) * 0.5;
            
            if (average.length() > 0.5) {
                int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                unsigned char r = pixels[n];
                unsigned char g = pixels[n + 1];
                unsigned char b = pixels[n + 2];
                
                ofColor col = ofColor(r, g, b);
                col = ofColor(127);
                /*
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, sat * 0.5, br * 2.0);
                 */
                
                Ribbon *p = new Ribbon();
                p->setup(ofVec3f(x + ofRandom(skip), y + ofRandom(skip), 0),
                         ofVec3f(ofRandom(-1, 1) * average.length(), ofRandom(-1, 1)  * average.length(), 20), col);
                p->friction = -0.01;
                p->radius = average.length() * 0.4 + 1.0;

                ribbons.push_back(p);
                
                if (ribbons.size() > 4000) {
                    delete ribbons[0];
                    ribbons.pop_front();
                }
            }
        }
        
        for (int i = 0; i < ribbons.size(); i++) {
            ribbons[i]->draw();
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    ofSetLineWidth(1);
    
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpRadial::getName(){
    return "opradial";
}
