

#include "SoftClip.h"

pdsp::SoftClip::SoftClip(){
    
    addInput("signal", input_signal);
    addInput("threshold", input_threshold);
    addOutput("output", output);
    updateOutputNodes();

    input_threshold.setDefaultValue( -3.0f );
    
    constant = 0;
    t = 1.0f;
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::SoftClip::set(float threshold){
    input_threshold.setDefaultValue(threshold);
    return *this;   
}

pdsp::Patchable& pdsp::SoftClip::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SoftClip::in_threshold(){
    return in("threshold");
}

pdsp::Patchable& pdsp::SoftClip::out_signal(){
    return out("signal");
}

void pdsp::SoftClip::prepareUnit( int expectedBufferSize, double sampleRate) {

}

void pdsp::SoftClip::releaseResources () {
    
}

void pdsp::SoftClip::process (int bufferSize) noexcept {

        int thresholdChanged;
        const float* thresholdBuffer = processInput(input_threshold, thresholdChanged);
        
        if(thresholdChanged){
            t = pow(10.0f, thresholdBuffer[0] * 0.05f); // db to lin 
            //t = 0.75;
            constant = -t + t*t;
        }

        int inputBufferState;
        const float* inputBuffer = processInput(input_signal, inputBufferState);

        if( inputBufferState == AudioRate){
            
            float* outputBuffer = getOutputBufferToFill(output);
            
            for (int n=0; n<bufferSize; ++n) {
                
                float xn = inputBuffer[n];
                
                float sign = (xn>0.0f) ? 1.0f : -1.0f;
                
                float yn = xn * sign; // abs value
                
                float saturated = (yn+constant) / yn;
                saturated *= sign; // sign mask
                
                yn = (yn>t) ? saturated : xn;
                
                outputBuffer[n] = yn;
            }
            
        }else{
            setOutputToZero(output);
        }

}
