#include "FatfontPixelate.h"
#include "ofApp.h"

void FatfontPixelate::setup(){
    font.loadFont("Rotunda.otf", 36, false);

    gui.setup();
    gui.add(radius.setup("Font Radius", 20, 1, 50));
    gui.add(srcLevel.setup("Font Level", 0, 0, 255));
    gui.loadFromFile("settings.xml");
}

void FatfontPixelate::update(){
    pixels = ((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels;
}

void FatfontPixelate::draw(){
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.begin();

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(srcLevel);
    tex.loadData(((ofApp*)ofGetAppPtr())->syphonIO.croppedPixels);
    tex.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    int camWidth = pixels.getWidth();
    int camHeight = pixels.getHeight();
    ofVec2f ratio;
    ratio.x = SCREEN_WIDTH / float(camWidth);
    ratio.y = SCREEN_HEIGHT / float(camHeight);

    ofPushMatrix();
    ofTranslate(0, radius * 2);

    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight; j+=radius){
                unsigned char r = pixels[(j * camWidth + i)*3];
                unsigned char g = pixels[(j * camWidth + i)*3+1];
                unsigned char b = pixels[(j * camWidth + i)*3+2];
                
                ofColor col;
                col = ofColor(r, g, b);
                int hue = col.getHue();
                int sat = col.getSaturation();
                int br = col.getBrightness();
                col.setHsb(hue, sat * 0.5, 255);
                
                ofSetColor(col);

                int brightness = 255 - ((r + g + b) / 3.0);
                int num = ofMap(brightness, 0, 255, 9, 0);
                string str = ofToString(num);
                font.drawString(str, i * ratio.x, j * ratio.y);
            }
        }
    }
    ofPopMatrix();
    
    ((ofApp*)ofGetAppPtr())->syphonIO.fbo.end();
    ((ofApp*)ofGetAppPtr())->syphonIO.server.publishTexture(&((ofApp*)ofGetAppPtr())->syphonIO.fbo.getTextureReference());
    
    ofBackground(0);
    gui.draw();
}

string FatfontPixelate::getName(){
    return "fatfont";
}