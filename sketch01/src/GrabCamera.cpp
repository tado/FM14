#include "GrabCamera.h"

void GrabCamera::setup(){
    camSize = ofVec2f(640,480);
    
	vector<ofVideoDevice> devices = cam.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    
    cam.setDeviceID(2);
    cam.setDesiredFrameRate(60);
    cam.initGrabber(camSize.x, camSize.y);
    camTexture = cam.getTextureReference();
}

void GrabCamera::update(){
    cam.update();
}
