#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1280, 960, OF_WINDOW);			// <-------- setup the GL context
    
    //ofAppGLFWWindow window;
    //window.setMultiDisplayFullscreen(true);
    //ofSetupOpenGL(&window,4080,768,OF_FULLSCREEN);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
