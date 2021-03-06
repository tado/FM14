#include "OpVbo.h"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void OpVbo::stateEnter(){
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //init VBO
    for (int i = 0; i < NUM; i++) {
        vels[i].set(0, 0, 0);
        colors[i].set(1, 1, 1);
        verts[i].set(0, 0, 0);
        force[i].set(0, 0, 0);
        velocity[i].set(0, 0, 0);
    }
    vbo.setVertexData(verts, NUM, GL_DYNAMIC_DRAW);
    vbo.setColorData(colors, NUM, GL_DYNAMIC_DRAW);
    initPos = false;
}

void OpVbo::stateExit(){
}

void OpVbo::setup() {
    cvWidth = 240;
    cvHeight = 45;
    
    int camWidth = ((ofApp*)ofGetAppPtr())->syphonIO.width;
    int camHeight = ((ofApp*)ofGetAppPtr())->syphonIO.height;
    pixels.allocate(camWidth, camHeight, 3);
    
    ofDisableArbTex();
    img.loadImage("particle32.png");
    
    // GUI
    gui.setup();
    gui.add(dual.setup("VBO dual", false));
    gui.add(fade.setup("VBO fade", 0.5, 0.0, 1.0));
    gui.add(accel.setup("VBO accel", 0.12, 0.0, 1.0));
    gui.add(friction.setup("VBO friction", 0.0, -0.1, 0.1));
    gui.add(hue.setup("VBO hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("VBO saturation", 1.0, 0.0, 5.0));
    gui.add(br.setup("VBO brightness", 1.0, 0.0, 5.0));
    gui.add(radius.setup("VBO radius", 4.0, 1.0, 32.0));
    gui.add(wind.setup("VBO wind", 0.0, -0.001, 0.001));
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

void OpVbo::update() {
    farneback.setPyramidScale(pyrScale);
    farneback.setNumLevels(levels);
    farneback.setWindowSize(winsize);
    farneback.setNumIterations(iterations);
    farneback.setPolyN(polyN);
    farneback.setPolySigma(polySigma);
    farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
    pixels.resize(cvWidth, cvHeight);
    farneback.calcOpticalFlow(pixels);
}

void OpVbo::draw() {
    int skip = 1;
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0,fade * 32);
    ofRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    
    if (farneback.getWidth() > 0) {
        ofVec2f scale = ofVec2f(SCREEN_WIDTH / float(farneback.getWidth()), SCREEN_HEIGHT / float(farneback.getHeight()));
        ofPushMatrix();
        ofScale(scale.x, scale.y);
        //ofTranslate(0, skip);
        
        for (int i = 0; i < NUM; i++) {
            
            // Dual screen
            if (dual) {
                
                int x, y;
                if (!initPos) {
                    x = verts[i].x = ofRandom(farneback.getWidth()-skip);
                    y = verts[i].y = ofRandom(farneback.getHeight()-skip);
                    int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                    unsigned char r = pixels[n];
                    unsigned char g = pixels[n + 1];
                    unsigned char b = pixels[n + 2];
                    
                    ofColor col = ofColor(r, g, b);
                    int h = col.getHue();
                    int s = col.getSaturation();
                    int v = col.getBrightness();
                    col.setHsb(h * hue, s * sat, v * br);
                    colors[i] = col;
                } else {
                    if (((verts[i].x > 0 && verts[i].x < farneback.getWidth() / 2.0 - 1.0)
                         || (verts[i].x > farneback.getWidth() / 2.0 + 1.0 && verts[i].x < farneback.getWidth()))
                        && verts[i].y > 0 && verts[i].y < farneback.getHeight()) {
                        x = verts[i].x;
                        y = verts[i].y;
                    } else {
                        x = verts[i].x = ofRandom(farneback.getWidth()-skip);
                        y = verts[i].y = ofRandom(farneback.getHeight()-skip);
                        verts[i].z = 0;
                        int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                        unsigned char r = pixels[n];
                        unsigned char g = pixels[n + 1];
                        unsigned char b = pixels[n + 2];
                        
                        ofColor col = ofColor(r, g, b);
                        int h = col.getHue();
                        int s = col.getSaturation();
                        int v = col.getBrightness();
                        col.setHsb(h * hue, s * sat, v * br);
                        colors[i] = col;
                    }
                }
                ofRectangle region = ofRectangle(x, y, skip, skip);
                ofVec2f average = farneback.getAverageFlowInRegion(region);
                
                force[i].set(0, 0, 0);
                force[i] -= velocity[i] * friction;
                force[i] += ofVec3f(average.x * accel, average.y * accel, 0);
                
                ofVec2f center;
                if (x < farneback.getWidth()/2.0) {
                    center.x = farneback.getWidth() / 6.0;
                    center.y = farneback.getHeight() / 2.0;
                } else {
                    center.x = farneback.getWidth() / 6.0 * 5.0;
                    center.y = farneback.getHeight() / 2.0;
                }
                
                force[i] += ofVec3f((x - center.x) * wind,
                                    (y - center.y) * wind,
                                    0.0);
                velocity[i] += force[i];
                verts[i] += velocity[i];
            } else {
                
                // Single screen
                
                int x, y;
                if (!initPos) {
                    x = verts[i].x = ofRandom(farneback.getWidth()-skip);
                    y = verts[i].y = ofRandom(farneback.getHeight()-skip);
                    int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                    unsigned char r = pixels[n];
                    unsigned char g = pixels[n + 1];
                    unsigned char b = pixels[n + 2];
                    
                    ofColor col = ofColor(r, g, b);
                    int h = col.getHue();
                    int s = col.getSaturation();
                    int v = col.getBrightness();
                    col.setHsb(h * hue, s * sat, v * br);
                    colors[i] = col;
                } else {
                    if (verts[i].x > 0 && verts[i].x < farneback.getWidth()
                        && verts[i].y > 0 && verts[i].y < farneback.getHeight()) {
                        x = verts[i].x;
                        y = verts[i].y;
                    } else {
                        x = verts[i].x = ofRandom(farneback.getWidth()-skip);
                        y = verts[i].y = ofRandom(farneback.getHeight()-skip);
                        verts[i].z = 0;
                        int n = ((y * camWidth + x) * 3) * camWidth / farneback.getWidth();
                        unsigned char r = pixels[n];
                        unsigned char g = pixels[n + 1];
                        unsigned char b = pixels[n + 2];
                        
                        ofColor col = ofColor(r, g, b);
                        int h = col.getHue();
                        int s = col.getSaturation();
                        int v = col.getBrightness();
                        col.setHsb(h * hue, s * sat, v * br);
                        colors[i] = col;
                    }
                }
                ofRectangle region = ofRectangle(x, y, skip, skip);
                ofVec2f average = farneback.getAverageFlowInRegion(region);
                
                force[i].set(0, 0, 0);
                force[i] -= velocity[i] * friction;
                force[i] += ofVec3f(average.x * accel, average.y * accel, 0);
                
                ofVec2f center;
                center.x = farneback.getWidth() / 2.0;
                center.y = farneback.getHeight() / 2.0;
                
                force[i] += ofVec3f((x - center.x) * wind,
                                    (y - center.y) * wind,
                                    0.0);
                velocity[i] += force[i];
                verts[i] += velocity[i];
            }
        }
        
        vbo.setVertexData(verts, NUM, GL_DYNAMIC_DRAW);
        vbo.setColorData(colors, NUM, GL_DYNAMIC_DRAW);
        
        img.getTextureReference().bind();
        ofSetColor(255);
        glPointSize(radius);
        ofEnablePointSprites();
        vbo.draw(GL_POINTS, 0, NUM);
        img.getTextureReference().unbind();
        ofDisablePointSprites();
        
        ofPopMatrix();
        initPos = true;
    }
    ofDisableBlendMode();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string OpVbo::getName(){
    return "opvbo";
}
