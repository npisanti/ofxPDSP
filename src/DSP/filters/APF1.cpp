
#include "APF1.h"

pdsp::APF1::APF1(){
    addInput("signal", input_signal);
    addOutput("signal", output);
    addInput("freq", input_freq);
    addInput("feedback", input_fb);
    updateOutputNodes();
    
    input_fb.setDefaultValue(0.0f);
    input_freq.setDefaultValue(440.0f);

    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::APF1::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::APF1::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::APF1::in_feedback(){
    return in("feedback");
}



void pdsp::APF1::prepareUnit( int expectedBufferSize, double sampleRate ) {
    alphaMultiplier = M_PI / sampleRate;
    x_z1_0 = 0.0f;
    y_z1_0 = 0.0f;
    fb_z1 = 0.0f;
}

void pdsp::APF1::releaseResources() {
    
}

void pdsp::APF1::process( int bufferSize ) noexcept {


    int inputBufferState;
    const float* inputBuffer = processInput(input_signal, inputBufferState);

    if(inputBufferState==AudioRate){

        int freqBufferState;
        const float* freqBuffer = processInput(input_freq, freqBufferState);
        int fbBufferState;
        const float* fbBuffer = processInput(input_fb, fbBufferState);
        
        int switcher = freqBufferState + fbBufferState*4;

        //process once
        switch (switcher & 21) {
        case 0 :
            process_once<false, false>( freqBuffer, fbBuffer );
            break;
        case 1 :
            process_once<true, false>( freqBuffer, fbBuffer );
            break;
        case 4 : 
            process_once<false, true>( freqBuffer, fbBuffer );
            break;
        case 5 :
            process_once<true, true>( freqBuffer, fbBuffer );
            break;
        default:
            break;
        }
            
        //process audio rate
        switch ( switcher & 42 ) {
        case 0 : 
            process_audio<false, false>(inputBuffer, freqBuffer, fbBuffer, bufferSize);
            break;
        case 2 : 
            process_audio<true, false>(inputBuffer, freqBuffer, fbBuffer, bufferSize);
            break;
        case 8 :
            process_audio<false, true>(inputBuffer, freqBuffer, fbBuffer, bufferSize);
            break;
        case 10 :
            process_audio<true, true>(inputBuffer, freqBuffer, fbBuffer, bufferSize);
            break;
        default:
            break;
        }
    }else{
            setOutputToZero(output);
    }
}

template<bool freqChange, bool fbChange>
void pdsp::APF1::process_once( const float* freqBuffer, const float* fbBuffer) noexcept {
    
    if(freqChange) {
        float tan_w0 = tanf( freqBuffer[0] * alphaMultiplier ) ;
        alpha = (tan_w0 - 1.0f) / (tan_w0 + 1.0f);
    }   
    
    if(fbChange){
        fb = fbBuffer[0];
    } 
}

template<bool freqAR, bool fbAR>
void pdsp::APF1::process_audio(const float* inputBuffer, const float* freqBuffer, const float* fbBuffer, const int &bufferSize)noexcept{
    
    float* outputBuffer = getOutputBufferToFill(output);
    
    for(int n=0; n<bufferSize; ++n){
    
        if(freqAR){ 
            float tan_w0 = tanf( freqBuffer[n] * alphaMultiplier ) ;
            alpha = (tan_w0 - 1.0f) / (tan_w0 + 1.0f);         
        }
        
        if(fbAR){
            fb = fbBuffer[n];
        }
        
        float xn = inputBuffer[n] + fb*fb_z1;
        
        //first stage
        float yn = alpha * (xn - y_z1_0) + x_z1_0;
        y_z1_0 = yn;
        x_z1_0 = xn;
        
        fb_z1 = yn;
        outputBuffer[n] = yn;
        
    }
    
}











