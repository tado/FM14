#include "OpParticle.h"
#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void OpParticle::stateEnter(){
    ofDisableBlendMode();
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OpParticle::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
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

void OpParticle::update() {
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
}

void OpParticle::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 15);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofSetCircleResolution(32);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if (farneback.getWidth() > 0) {
        int skip = 1;
        
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        ofTranslate(0, skip);
        
        //int num = 0;
        for (int i = 0; i < 10000; i++) {
            //while (num < 300){
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region);
            float radius = (avrage.x + avrage.y) * 1.0;
            int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            ofColor col = ofColor(r, g, b);
            int hue = col.getHue();
            int sat = col.getSaturation();
            int br = col.getBrightness();
            
            if (br > 31) {
                col.setHsb(hue, sat, 255);
                ofSetColor(col);
                
                if (abs(radius) > skip * 4.0) {
                    radius = skip * 4.0;
                }
                ofEnableSmoothing();
                ofCircle(x + ofRandom(-skip,skip), y + ofRandom(-skip,skip), radius);
                ofDisableSmoothing();
                //num++;
            }
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ofSetColor(255);
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.draw(0, 0);
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    //((testApp*)ofGetAppPtr())->syphonIO.server.publishScreen();
}

string OpParticle::getName(){
    return "opparticle";
}
