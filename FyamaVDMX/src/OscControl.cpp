#include "OscControl.h"
#include "testApp.h"

void OscControl::setup(){
    receiver.setup(9000);
}

void OscControl::update(){
    while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        cout << "OSC = " << &m << endl;
        
		if(m.getAddress() == "/FromVDMX/state"){
            state = m.getArgAsInt32(0);
            switch (state) {
                case 0:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("blank");
                    break;
                case 1:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("simple");
                    break;
                case 2:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("box");
                    break;
                case 3:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("fatfont");
                    break;
                case 4:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opcircle");
                    break;
                case 5:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opparticle");
                    break;
                case 6:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opvector");
                    break;
                case 7:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opdistort");
                    break;
                case 8:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opradial");
                    break;
                case 9:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("tile");
                    break;
            }
		}
    }
}