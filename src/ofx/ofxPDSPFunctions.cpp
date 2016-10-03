
#include "ofxPDSPFunctions.h"

void ofxPDSPSetup(int bufferSize, double sampleRate){
    pdsp::prepareAllToPlay(bufferSize, sampleRate);
}

void ofxPDSPRelease(){
    pdsp::releaseAll();
}

float p2f( float pitch){
    return pow( 2.0, (pitch-69.0)/12.0 ) * pdsp::PitchToFreq::getTuningFrequency();
}

float f2p( float freq){
    return (log2( freq / pdsp::PitchToFreq::getTuningFrequency()) * 12.0f) + 69.0f;
}

float pratio ( float pitch, float ratio ){
    float f = p2f ( pitch );
    f = f*ratio;
    return f2p(f);
}

int highestPartial( float pitch ) {
    
    float freq = p2f(pitch);

    int partial = 1;
    float partialFreq = freq;
    
    while( partialFreq < 11050.0f ){
        partial++;
        partialFreq = freq * (float)partial;
    } 
    
    return partial;
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

float pdspSpread(int index, int max, float spread, bool fromCenter){
    if(fromCenter){
        float offset = spread / static_cast<float>(max/2);
        float mult;
        if(max%2==0){
            mult = static_cast<float>((index+2)/2);
        }else{
            mult = static_cast<float>((index+1)/2);
        }
        if(index%2==0){
            return -mult*offset;
        }else{
            return mult*offset;
        }
    }else{
        float offset = ((float)(max-1))*0.5f;
        float pan = (float(index)) - offset; 
        float panScale = spread / offset; 
        return pan * panScale;        
    }
    

}
