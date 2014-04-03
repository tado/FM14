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
    gui.add(skip.setup("Particle skip", 1, 1, 20));
    gui.add(thresh.setup("Particle thresh", 5, 0, 10));
    gui.add(srcLevel.setup("Particle Level", 0, 0, 255));
    gui.add(fade.setup("Particle fade", 12, 0, 63));
    gui.add(hue.setup("Particle hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Particle saturation", 1.0, 0.0, 5.0)); 
    gui.add(br.setup("Particle brightness", 1.0, 0.0, 3.0));
    gui.add(alpha.setup("Particle alpha", 0.8, 0.0, 1.0));
    gui.add(num.setup("Particle num", 1000, 10, 20000));
    gui.loadFromFile("settings.xml");
    
    //CV params
    pyrScale = 0.5;
    levels = 4;
    winsize = 8;
    iterations = 2;
    polyN = 7;
    polySigma = 1.5;
    OPTFLOW_FARNEBACK_GAUSSIAN = false;
    
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
}

void OpParticle::update() {
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
}

void OpParticle::draw() {
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, srcLevel);
    tex.loadData(((testApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, fade);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofSetCircleResolution(32);
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    if (farneback.getWidth() > 0) {
        //int skip = 1;
        
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        ofTranslate(0, skip);
        
        for (int i = 0; i < num; i++) {
            int x = ofRandom(farneback.getWidth()-skip);
            int y = ofRandom(farneback.getHeight()-skip);
            ofRectangle region = ofRectangle(x, y, skip, skip);
            ofVec2f avrage = farneback.getAverageFlowInRegion(region);
            if (avrage.length() > thresh) {
                avrage = ofVec2f(0, 0);
            }
            float radius = (avrage.x + avrage.y) * 1.0;
            int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
            unsigned char r = pixels[n];
            unsigned char g = pixels[n + 1];
            unsigned char b = pixels[n + 2];
            
            ofColor col = ofColor(r, g, b);
            int h = col.getHue();
            int s = col.getSaturation();
            int v = col.getBrightness();
            col.setHsb(h * hue, s * sat, v * br);
            
            ofSetColor(col, alpha * 255);
            //ofEnableSmoothing();
            ofCircle(x + ofRandom(-skip,skip), y + ofRandom(-skip,skip), radius);
            //ofDisableSmoothing();
        }
        ofPopMatrix();
    }
    ofDisableBlendMode();
    
    ((testApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((testApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((testApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpParticle::getName(){
    return "opparticle";
}
