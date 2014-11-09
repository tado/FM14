#pragma once
#include "ofMain.h"
#include "ofxFft.h"

class FFTData {
public:
    FFTData(int bufferSize);
    void update();
    void draw();
    void audioReceived(float* input, int bufferSize, int nChannels);
    
    ofxFft* fft;
    int bufferSize;
    ofMutex soundMutex;
    float maxValue;
    vector<float> drawBins, middleBins, audioBins;
};