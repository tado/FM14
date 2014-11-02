#include "OpColorParticle.h"
#include "ofApp.h"

void OpColorParticle::setup(){
    mesh.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(3.0);
    
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("OP COLOR PARTICLE");
    gui->addSpacer();
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("simple_pixelate.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&OpColorParticle::guiEvent);
    
    //パーティクルを生成
    for (int i = 0; i < NUM; i++){
        Particle myParticle;
        myParticle.friction = 0.005;
        myParticle.setup(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), ofVec2f(0, 0));
        particles.push_back(myParticle);
    }
    
    //OPTFLOW_FARNEBACK_GAUSSIAN = false;
}

void OpColorParticle::update(){
    pixels = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    mesh.clear();
    
    if(((ofApp*)ofGetAppPtr())->blackmagic->cam.update()) {
        flow.setPyramidScale(0.5);
        flow.setNumLevels(4);
        flow.setWindowSize(8);
        flow.setNumIterations(2);
        flow.setPolyN(7);
        flow.setPolySigma(1.5);
        flow.setUseGaussian(cv::OPTFLOW_FARNEBACK_GAUSSIAN);
        
        flow.calcOpticalFlow(pixels);
    }
    
    // Particleのアップデート
    for (int i = 0; i < particles.size(); i++){
        
        //particleの力をリセット
        particles[i].resetForce();
        
        //オプティカルフローから、それぞれのparticleにかかる力を算出
        ofVec2f force;
        ofVec2f pos;
        pos.x = particles[i].position.x;
        pos.y = particles[i].position.y;
        if (pos.x > 0 && pos.y > 0) {
            force = flow.getFlowOffset(pos.x, pos.y) * flowScale;
        }
        
        //VBO mesh更新
        mesh.addVertex(ofVec3f(particles[i].position.x, particles[i].position.y, 0));
        
        //Particleの状態を更新
        particles[i].addForce(ofVec2f(force.x, force.y));
        particles[i].updateForce();
        particles[i].update();
        particles[i].throughOfWalls();
    }
    
}

void OpColorParticle::draw(){
    ofSetColor(255, 100);
    mesh.draw();
}

void OpColorParticle::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("simple_pixelate.xml");
    }
}

void OpColorParticle::keyPressed(int key){
    if (key == 'g') {
        gui->toggleVisible();
    }
}

string OpColorParticle::getName(){
    return "opcolorparticle";
}