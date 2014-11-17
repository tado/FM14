#include "StFftSphereStripe.h"
#include "ofApp.h"

string StFftSphereStripe::getName(){
    return "StFftSphereStripe";
}

void StFftSphereStripe::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("STRIPE SPHERE");
    gui->addSpacer();
    gui->addSlider("TOP SHIFT", 0, 100.0, 50.0);
    gui->addSlider("NOISE SCALE", 1.0, 30.0, 10.0);
    gui->addSlider("SHIFT SPEED", 0.0, 2.0, 1.0);
    gui->addSlider("ZOOM", 0.0, 3.0, 1.0);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 1.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftSphereStripe.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftSphereStripe::guiEvent);
    app = ((ofApp*)ofGetAppPtr());
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    
    cam.setFarClip(10000);

    int width = 512;
    int height = 512;
    unsigned char pixels[width * height * 4];
    
    for (int i = 0; i < width * height * 4; i += 4){
        pixels[i] = pixels[i+1] = pixels[i+2] = 255;
        if (i % 16 == 0) {
            pixels[i + 3] = 255;
        } else {
            pixels[i + 3] = 0;
        }
    }
    tex.loadData(pixels, width, height, GL_RGBA);
    createMesh();
}

void StFftSphereStripe::update(){
    ofxUISlider *gnoisescale = (ofxUISlider *)gui->getWidget("NOISE SCALE"); float noisescale = gnoisescale->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    
    float distortionStrength = ofMap(app->oscControl->controlVal[2], 0, 127, 1, 8);
    
    float fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    for (int i = 0; i < mesh.getVertices().size(); i++) {
        float noiseX = ofMap(currentVertex[i].x, 0, ofGetWidth(), 0, noisescale);
        float noiseY = ofMap(currentVertex[i].y, 0, ofGetWidth(), 0, noisescale);
        float offset = ofNoise(noiseX + ofGetElapsedTimef() * shiftspeed,
                               noiseY + ofGetElapsedTimef() * shiftspeed);
        currentVertex[i] = currentVertex[i].normalize() * (offset * fftSum * distortionStrength + ofGetWidth() / 32.0);
        mesh.setVertex(i, currentVertex[i]);
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StFftSphereStripe::draw(){
    ofxUISlider *gtopshift = (ofxUISlider *)gui->getWidget("TOP SHIFT"); float topshift = gtopshift->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    
    app->drawFbo->fbo.begin();
    post.begin(cam);
    ofTranslate(0, app->drawFbo->top / 2.0);
    ofScale(zoom, zoom);
    ofRotateX(ofGetElapsedTimef() * shiftspeed);
    ofRotateY(ofGetElapsedTimef() * shiftspeed * 1.1);
    ofRotateZ(ofGetElapsedTimef() * shiftspeed * 1.2);
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    float controlHue;
    controlHue = ofMap(app->oscControl->controlVal[3], 0, 127, 0, 1);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    //ofEnableDepthTest();
    glDisable(GL_CULL_FACE);
    tex.bind();
    mesh.draw();
    //ofRotateX(shiftspeed * ofGetElapsedTimef() * 0.2 + 40);
    //ofRotateY(shiftspeed * ofGetElapsedTimef() * 0.3 + 40);
    //ofRotateZ(shiftspeed * ofGetElapsedTimef() * 0.5 + 40);
    
    ofRotateX(10);
    ofRotateY(12);
    ofRotateZ(14);
    
    col.setHsb(int(controlHue * 255 + 127) % 255, sat * 255, br * 255);
    ofSetColor(col);
    mesh.draw();
    tex.unbind();
    //mesh.drawWireframe();
    //ofDisableDepthTest();
    post.end();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StFftSphereStripe::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftSphereStripe.xml");
    }
}

void StFftSphereStripe::createMesh(){
    //mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh = ofSpherePrimitive(ofGetWidth(), 48).getMesh();
    for (int i = 0; i < mesh.getVertices().size(); i++) {
        ofVec2f texCoord = mesh.getTexCoord(i);
        texCoord.x *= tex.getWidth();
        texCoord.y  = (1.0 - texCoord.y) * tex.getHeight();
        mesh.setTexCoord(i, texCoord);

        currentVertex.push_back(ofVec3f(mesh.getVertices()[i].x, mesh.getVertices()[i].y, mesh.getVertices()[i].z));
    }
}

void StFftSphereStripe::stateExit(){
    gui->setVisible(false);
}