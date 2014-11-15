#include "StFftTracer.h"

class Tracker
{
public:
    deque<ofVec3f> points;
    float trackerLength;
    
    void setup(){
        
    }
    
    void update(ofVec3f _pos) {
        ofVec3f p = _pos;
        
        //if (p.distance(points.front()) > 1){
        points.push_front(p);
        //}
        
        if (points.size() > trackerLength){
            points.pop_back();
        }
    }
    
    void draw()	{
        if (points.empty()) return;
        
        /*
         for (int i = 0; i < points.size() - 1; i++){
         float a = ofMap(i, 0, points.size() - 1, 1, 0);
         
         ofVec3f &p0 = points[i];
         ofVec3f &p1 = points[i + 1];
         
         float dist = p0.distance(p1);
         
         //if (dist < 40) {
         //ofSetLineWidth(ofMap(dist, 0, 30, 0, 10));
         //ofSetColor(dist*20, 127-dist*10, 255-dist*20);
         ofLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
         //}
         }
         */
        //ofSetLineWidth(1);
        ofBeginShape();
        for (int i = 0; i < points.size() - 1; i++){
            //ofVertex(points[i].x, points[i].y, points[i].z);
            ofCurveVertex(points[i].x, points[i].y, points[i].z);
        }
        //ofVertex(points[0].x, points[0].y, points[0].z);
        ofEndShape();
    }
    
    void clear() {
        points.clear();
    }
    
    void setTrackerLength(float _trackerLength) {
        trackerLength = _trackerLength;
    }
};

vector<Tracker*> trackers;

string StFftTracer::getName(){
    return "StFftTracer";
}

void StFftTracer::setup() {
    app = ((ofApp*)ofGetAppPtr());
    fft_size = app->bufferSize;
    
    for (int i = 0; i < fft_size; i++) {
        ofVec3f p = ofVec3f(0, 0, 0);
        pos.push_back(p);
        ofVec3f v = ofVec3f(0, 0, 0);
        vel.push_back(v);
        
        Tracker *t = new Tracker;
        t->setup();
        t->setTrackerLength(8);
        trackers.push_back(t);
    }
    
    //ばねパラメータ
    /*
     stiffness = 1.0;
     damping = 0.9;
     mass = 100.0;
     */
    stiffness = 1.0;
    damping = 0.5;
    mass = 10;
    
    //((ofApp*)ofGetAppPtr())->cam.setPosition(0, 0, -200);
    //((ofApp*)ofGetAppPtr())->cam.lookAt(ofVec3f(0,0,0));
    //camStart = ofPoint(0,0,-200);
    //camEnd = camStart;
}

void StFftTracer::update(){
    for (int i=0; i < fft_size; i++) {
        float magnitude = powf(app->fft->drawBins[i], 0.4);
        float addFroce;
        addFroce = magnitude * 1500.0;
        float direction = ofRandom(360);
        ofVec3f add = ofVec3f(cos(direction) * addFroce, sin(direction) * addFroce,  ofRandom(-1,1)*addFroce);
        ofVec3f force = stiffness * -pos[i] + add;
        ofVec3f acceleration = force / mass;
        vel[i] = damping * (vel[i] + acceleration);
        pos[i] += vel[i];
        
        trackers[i]->update(pos[i]);
    }
}

void StFftTracer::draw(){
    app->drawFbo->fbo.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    cam.begin();
    glEnable(GL_DEPTH_TEST);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofPushMatrix();
    ofScale(1.0, 1.0, 1.0);
    ofRotateX(ofGetElapsedTimef() * 4);
    ofRotateY(ofGetElapsedTimef() * 3);
    ofRotateZ(ofGetElapsedTimef() * 2);
    //ofNoFill();
    for (int i=0; i  < trackers.size(); i++) {
        ofSetColor(ofColor::fromHsb(255 * i / fft_size, 200, 150));
        trackers[i]->draw();
    }
    //ofFill();
    ofPopMatrix();
    glDisable(GL_DEPTH_TEST);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}
