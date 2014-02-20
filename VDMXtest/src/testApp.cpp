#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    
	dir.setup();
    client.setup();
    
    ofAddListener(dir.events.serverAnnounced, this, &testApp::serverAnnounced);
    //ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &testApp::serverRetired);
    camSize.x = ofGetWidth();
    camSize.y = ofGetHeight();
    fboSyphonIn.allocate(camSize.x, camSize.y, GL_RGB);
    fboSyphonOut.allocate(camSize.x, camSize.y, GL_RGBA);
    pix.allocate(camSize.x, camSize.y, 3);
    dirIdx = -1;
}

void testApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void testApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void testApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();
    
    if(dir.isValidIndex(dirIdx)){
        ofSetColor(255);
        fboSyphonIn.begin();
        {
            ofClear(0,0,0,0);
            client.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        fboSyphonIn.end();
        fboSyphonIn.getTextureReference().readToPixels(pix);
        pix.resize(640, 480);
        
        //fboSyphonOut.begin();
        int radius = 20;
        ofVec2f scale;
        scale.x = ofGetWidth() / float(pix.getWidth());
        scale.y = ofGetHeight() / float(pix.getHeight());
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(radius/2, radius/2);
        unsigned char * pixels = pix.getPixels();
        for (int i = 0; i < pix.getWidth(); i+=radius){
            for (int j = 0; j < pix.getHeight(); j+=radius){
                unsigned char r = pixels[(j * int(pix.getWidth()) + i)*3];
                unsigned char g = pixels[(j * int(pix.getWidth()) + i)*3+1];
                unsigned char b = pixels[(j * int(pix.getWidth()) + i)*3+2];
                
                ofSetColor(255, 0, 0, 180);
                ofRect((i - radius / 4.0) * scale.x, j * scale.y,radius*(float)r/255.0 * scale.x, radius*(float)r/255.0 * scale.x);
                ofSetColor(0, 255, 0, 180);
                ofRect(i * scale.x, j * scale.y, radius*(float)g/255.0 * scale.x, radius*(float)g/255.0 * scale.x);
                ofSetColor(0, 0, 255, 180);
                ofRect((i + radius / 4.0) * scale.x, j * scale.y, radius*(float)b/255.0 * scale.x, radius*(float)b/255.0 * scale.x);
                
            }
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
        //fboSyphonOut.end();
        //ofSetColor(255);
        //fboSyphonOut.draw(0, 0); //4. draw the texture to the OF screen - this is optional since we have our texture now...it can just be sent back out
        //server.publishTexture(&fboSyphonOut.getTextureReference());
        server.publishScreen();
        ofDisableBlendMode();
    }
    
    ofSetColor(255);
    ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate(),4), 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //press any key to move through all available Syphon servers
    dirIdx++;
    if(dirIdx > dir.size() - 1){
        dirIdx = 0;
    }
    
    client.set(dir.getDescription(dirIdx));
    string serverName = client.getServerName();
    string appName = client.getApplicationName();
    
    if(serverName == ""){
        serverName = "null";
    }
    if(appName == ""){
        appName = "null";
    }
    ofSetWindowTitle(serverName + ":" + appName);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
