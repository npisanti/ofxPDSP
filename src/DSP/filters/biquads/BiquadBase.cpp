
#include "BiquadBase.h"


pdsp::BiquadBase::BiquadBase(){
    addInput("signal", input_signal);
    addOutput("signal", output);
    addInput("freq", input_frequency);
    addInput("Q", input_Q);
    addInput("gain", input_gain);
    updateOutputNodes();
    
    input_gain.setDefaultValue(0.0f);
    input_Q.setDefaultValue(0.707f);
    input_frequency.setDefaultValue(8000.0f);
    
    BiquadBase::cookCoefficients(0.0f, 0.0f, 0.0f, 0.0f);
}

pdsp::Patchable& pdsp::BiquadBase::set(float freq, float Q, float gain){
    input_frequency.setDefaultValue(freq);
    input_Q.setDefaultValue(Q);
    input_gain.setDefaultValue(gain);
    return *this;
}

pdsp::Patchable& pdsp::BiquadBase::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::BiquadBase::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::BiquadBase::in_Q(){
    return in("Q");
}


pdsp::Patchable& pdsp::BiquadBase::in_gain(){
    return in("gain");
}

pdsp::Patchable& pdsp::BiquadBase::out_signal(){
    return out("signal");
}

void pdsp::BiquadBase::cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain){
    //this function has to be overloaded, with this variables the filter does nothing, just let the signal pass
    //remember to divide all the coeff for a0
    b0 = 1.0f;
    b1 = 0.0f;
    b2 = 0.0f;
    a1 = 0.0f;
    a2 = 0.0f;
}

void pdsp::BiquadBase::prepareUnit( int expectedBufferSize, double sampleRate ) {
    this->sampleRate = sampleRate;
    x_z1 = 0.0f;
    x_z2 = 0.0f;
    y_z1 = 0.0f;
    y_z2 = 0.0f;
}

void pdsp::BiquadBase::releaseResources () {}

void pdsp::BiquadBase::process (int bufferSize) noexcept {
        
    int inputState;
    const float* inputBuffer = processInput(input_signal, inputState); 
    
    if(inputState==AudioRate){
    
        int changed;
        const float * qBuffer = processInput(input_Q, changed);
    
        int freqState;
        const float * freqBuffer = processInput(input_frequency, freqState);
        changed += freqState;
    
        int gainState;
        const float * gainBuffer = processInput(input_gain, gainState);
        changed += gainState;
                
        if(changed){//true if freq or Q is changed
            cookCoefficients( sampleRate, freqBuffer[0], qBuffer[0], gainBuffer[0] );
        }
    
        float* outputBuffer = getOutputBufferToFill(output);
        
        for(int n=0; n<bufferSize; ++n){
            //direct form 1
            float xn = inputBuffer[n];

            float yn = b0*xn + b1*x_z1 + b2*x_z1 
                             - a1*y_z1 - a2*y_z2;
                               
            x_z2 = x_z1;
            x_z1 = xn;
            y_z2 = y_z1;
            y_z1 = yn;

            outputBuffer[n] = yn;
        }                                
    }else{
        setOutputToZero(output);
    }

}  
