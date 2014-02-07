#include "GrabCamera.h"
#include "testApp.h"

void GrabCamera::setup(){
    camSize.x = ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().camSize.x;
    camSize.y = ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().camSize.y;
    //camSize.x = 160;
    //camSize.y = 120;
    camSize = ofVec2f(camSize.x, camSize.y);
    
	vector<ofVideoDevice> devices = cam.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(60);
    cam.initGrabber(camSize.x, camSize.y);
    camTexture = cam.getTextureReference();
}

void GrabCamera::update(){
    cam.update();
}
