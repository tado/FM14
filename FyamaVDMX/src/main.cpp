#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    //ofSetupOpenGL(3610, 680, OF_WINDOW);
    //ofSetupOpenGL(3840, 720, OF_WINDOW);
	//ofSetupOpenGL(2560, 480, OF_WINDOW);
    //ofSetupOpenGL(2880, 540, OF_WINDOW);
    ofSetupOpenGL(720, 480, OF_WINDOW);
    
    //ofAppGLFWWindow window; // create a window
    //window.setMultiDisplayFullscreen(true);
    //ofSetupOpenGL(&window, SCREEN_WIDTH, SCREEN_HEIGHT, OF_FULLSCREEN);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
