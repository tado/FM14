#include "GrabCamera.h"
#include "testApp.h"

void GrabCamera::setup(){
    camSize.x = ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().camSize.x;
    camSize.y = ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().camSize.y;
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
    float sy = camSize.y / 24.0 * 3.0;
    camTexture = cam.getTextureReference();
}

void GrabCamera::update(){
    cam.update();
}
