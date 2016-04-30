
#include "ofxPDSPFunctions.h"

void ofxPDSPSetup(int bufferSize, double sampleRate){
    pdsp::prepareAllToPlay(bufferSize, sampleRate);
}

void ofxPDSPRelease(){
    pdsp::releaseAll();
}

float p2f( float pitch){
    return pdsp::PitchToFreq::eval( pitch );
}

void setReferenceFrequency(float frequency){
    pdsp::PitchToFreq::setTuning(frequency);
}


float panL ( float pan ){
    float theta;
    if(pan>=1.0f){
        theta = M_PI_2;
    }else if(pan<=-1.0f){
        theta = 0.0f;
    }else{
        theta = M_PI_4 * (pan+1.0f);        
    }
    return cosf(theta);
}


float panR ( float pan ){
    float theta;
    if(pan>=1.0f){
        theta = M_PI_2;
    }else if(pan<=-1.0f){
        theta = 0.0f;
    }else{
        theta = M_PI_4 * (pan+1.0f);        
    }
    return sinf(theta);
}

float dB ( float dBvalue ){
    return pdsp::DBtoLin::eval(dBvalue);
}

float pdspSpread(int index, int max, float spread){
    float offset = ((float)(max-1))*0.5f;
    float pan = (float(index)) - offset; 
    float panScale = spread / offset; 
    return pan * panScale;
}
