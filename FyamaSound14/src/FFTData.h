#pragma once
#include "ofMain.h"
#include "ofxFft.h"

class FFTData {
public:
    FFTData(int bufferSize);
    void audioReceived(float* input, int bufferSize, int nChannels);
    void draw();
    
    ofxFft* fft;
    int bufferSize;
    ofMutex soundMutex;
    vector<float> drawBins, middleBins, audioBins;
};