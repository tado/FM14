#include "OscControl.h"
#include "testApp.h"

void OscControl::setup(){
    receiver.setup(9000);
}

void OscControl::update(){
    while(receiver.hasWaitingMessages()){

		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		if(m.getAddress() == "/FromVDMX/state"){
            state = m.getArgAsInt32(0);
            switch (state) {
                case 0:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("simple");
                    break;
                case 1:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("box");
                    break;
                case 2:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("fatfont");
                    break;
                case 3:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opcircle");
                    break;
                case 4:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opparticle");
                    break;
                case 5:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opvector");
                    break;
                case 6:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opdistort");
                    break;
                case 7:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opradial");
                    break;
                case 8:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("tile");
                    break;
            }
		}
        if(m.getAddress() == "/FromVDMX/tile"){
            int div = m.getArgAsInt32(0) + 1;
            ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().tileDiv = div;
        }
    }
}