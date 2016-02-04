
#include "ofxPDSPFunctions.h"

void ofxPDSPSetup(int bufferSize, double sampleRate){
    pdsp::prepareAllToPlay(bufferSize, sampleRate);
}

void ofxPDSPRelease(){
    pdsp::releaseAll();
}
