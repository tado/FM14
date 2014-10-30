#include "BoxPixelate.h"
#include "ofApp.h"

void BoxPixelate::setup(){
    int camWidth = 1920;
    int camHeight = 351;
    radius = 10;
    num = camWidth * camHeight / radius;
    
    for (int i = 0; i<num; i++) {
        float a = ofRandom(360);
        float l = ofRandom(10);
        angle.push_back(a);
        length.push_back(l);
    }
    
    gui.setup();
    gui.add(radius.setup("Box Radius", 20, 1, 50));
    gui.add(srcLevel.setup("Box Level", 0, 0, 255));
    gui.add(rectScale.setup("Box scale", 8.0, 1.0, 10.0));
    gui.add(hue.setup("Box hue", 1.0, 0.0, 3.0));
    gui.add(sat.setup("Box saturation", 1.0, 0.0, 5.0));
    gui.add(br.setup("Box brightness", 1.0, 0.0, 3.0));
    gui.loadFromFile("settings.xml");
}

void BoxPixelate::update(){
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void BoxPixelate::draw(){
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(srcLevel);
    tex.loadData(((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    float width = pixels.getWidth();
    float height = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = SCREEN_WIDTH / float(width);
    ratio.y = SCREEN_HEIGHT / float(height);
    
    ofPushMatrix();
    ofTranslate(radius / 2.0 * ratio.x, radius / 2.0 * ratio.y);
    float scale = 0.01;
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (pixels.size()>0 && angle.size() > 0 && length.size() > 0){
        for (int i = 0; i < width; i+=radius){
            for (int j = 0; j < height; j+=radius){
                unsigned char r = pixels[(j * width + i)*3];
                unsigned char g = pixels[(j * width + i)*3+1];
                unsigned char b = pixels[(j * width + i)*3+2];
                
                ofColor col = ofColor(r, g, b);
                int h = col.getHue();
                int s = col.getSaturation();
                int v = col.getBrightness();
                col.setHsb(h * hue, s * sat, v * br);
                ofSetColor(col);
                
                ofPushMatrix();
                ofTranslate(i * ratio.x, j * ratio.y);
                
                int n = j * width / radius + i / radius;
                
                float curAngle = angle[n] + ((r + g + b) * 2.0 - angle[n]) / 60.0;
                ofRotateX(curAngle);
                ofRotateY(curAngle * 1.2);
                ofRotateZ(curAngle * 0.2);
                angle[n] = curAngle;
                
                float curLength = length[n] + ((r + g + b) * 1.0 - length[n]) / 20.0;
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofRect(0, 0, radius * rectScale, radius * rectScale / 5.0);
                ofSetRectMode(OF_RECTMODE_CORNER);
                length[n] = curLength;
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
    ofPopMatrix();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string BoxPixelate::getName(){
    return "box";
}