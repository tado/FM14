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
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("blank");
                    break;
                case 1:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("sakura2");
                    break;
                case 2:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opdistort");
                    break;
                case 3:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opbluevector");
                    break;
                case 4:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opredvector");
                    break;
                case 5:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opvector");
                    break;
                case 6:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opvbo");
                    break;
                case 7:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opnotes");
                    break;
                case 8:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("simple");
                    break;
                case 9:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("nofill");
                    break;
                case 10:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("box");
                    break;
                case 11:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("fatfont");
                    break;
                case 12:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opcircle");
                    break;
                case 13:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("opparticle");
                    break;
                case 14:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("bubble");
                    break;
                case 15:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("sakura2");
                    break;
                case 16:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("tile");
                    break;
                case 17:
                    ((testApp*)ofGetAppPtr())->stateMachine.changeState("movetile");
                    break;
            }
		}
        if(m.getAddress() == "/FromVDMX/tile"){
            int div = m.getArgAsInt32(0) + 1;
            ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().tileDiv = div;
        }
        if(m.getAddress() == "/FromVDMX/cropPosY"){
            ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().cropPosY = m.getArgAsFloat(0);
        }
        if (m.getAddress() == "/FromVDMX/threeHead") {
            ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().threeHead = m.getArgAsInt32(0);
            //((testApp*)ofGetAppPtr())->stateMachine.changeState("blank");
        }
        if (m.getAddress() == "/FromVDMX/redblue") {
            if(((testApp*)ofGetAppPtr())->stateMachine.getSharedData().redBlue){
                ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().redBlue = false;
            } else {
                ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().redBlue = true;
            }
        }
        if(m.getAddress() == "/FromVDMX/particlenum"){
            ((testApp*)ofGetAppPtr())->stateMachine.getSharedData().particleNum = m.getArgAsFloat(0);
        }
    }
}