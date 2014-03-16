#include "ofxState.h"
#include "SharedData.h"

class Tile : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    string getName();
};