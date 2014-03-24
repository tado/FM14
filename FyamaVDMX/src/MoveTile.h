#include "ofxState.h"
#include "SharedData.h"

class MoveTile : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    string getName();
    float tilePos;
    float width, height;
};