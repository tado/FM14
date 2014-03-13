#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
    ofSetupOpenGL(3610, 680, OF_WINDOW);
    //ofSetupOpenGL(3840, 720, OF_WINDOW);
	//ofSetupOpenGL(2560, 480, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
