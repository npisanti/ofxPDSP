
#include "APF4.h"

pdsp::APF4::APF4(){
    addInput("signal", input_signal);
    addOutput("signal", output);
    addInput("freq", input_freq);
    addInput("feedback", input_fb);
    addInput("spread", input_spread);
    updateOutputNodes();
    
    input_fb.setDefaultValue(0.0f);
    input_freq.setDefaultValue(440.0f);
    input_spread.setDefaultValue(0.98f);

    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::APF4::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::APF4::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::APF4::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::APF4::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::APF4::in_spread(){
    return in("spread");
}


void pdsp::APF4::prepareUnit( int expectedBufferSize, double sampleRate ) {
    alphaMultiplier = M_PI / sampleRate;
    x_z1_0 = 0.0f;
    x_z1_1 = 0.0f;
    x_z1_2 = 0.0f;
    x_z1_3 = 0.0f;
    y_z1_0 = 0.0f;
    y_z1_1 = 0.0f;
    y_z1_2 = 0.0f;
    y_z1_3 = 0.0f;

    fb_z1 = 0.0f;
}

void pdsp::APF4::releaseResources() {
    
}

void pdsp::APF4::process( int bufferSize ) noexcept {


    int inputBufferState;
    const float* inputBuffer = processInput(input_signal, inputBufferState);

    if(inputBufferState==AudioRate){

        int freqBufferState;
        const float* freqBuffer = processInput(input_freq, freqBufferState);
        int fbBufferState;
        const float* fbBuffer = processInput(input_fb, fbBufferState);
        int spreadBufferState;
        const float* spreadBuffer = processInput(input_spread, spreadBufferState);
        
        int switcher = freqBufferState + fbBufferState*4 + spreadBufferState*16;

        //process once
        switch (switcher & 21) {
        case 0 :
            process_once<false, false, false>( freqBuffer, fbBuffer, spreadBuffer);
            break;
        case 1 :
            process_once<true, false, false>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 4 : 
            process_once<false, true, false>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 5 :
            process_once<true, true, false>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 16 :
            process_once<false, false, true>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 17 : 
            process_once<true, false, true>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 20 : 
            process_once<false, true, true>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        case 21 : 
            process_once<true, true, true>( freqBuffer, fbBuffer, spreadBuffer );
            break;
        default:
            break;
        }
            
        //process audio rate
        switch ( switcher & 42 ) {
        case 0 : 
            process_audio<false, false, false>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 2 : 
            process_audio<true, false, false>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 8 :
            process_audio<false, true, false>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 10 :
            process_audio<true, true, false>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 32 : 
            process_audio<false, false, true>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 34 : 
            process_audio<true, false, true>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 40 :
            process_audio<false, true, true>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        case 42 :
            process_audio<true, true, true>(inputBuffer, freqBuffer, fbBuffer, spreadBuffer, bufferSize);
            break;
        default:
            break;
        }
    }else{
            setOutputToZero(output);
    }
}

template<bool freqChange, bool fbChange, bool spreadChange>
void pdsp::APF4::process_once( const float* freqBuffer, const float* fbBuffer, const float* spreadBuffer) noexcept {
    
    if(spreadChange){
        freqMult[0] = 1.0f;
        freqMult[1] = 1.0f + spreadBuffer[0];
        freqMult[2] = 1.0f + spreadBuffer[0]*2.0f;
        freqMult[3] = 1.0f + spreadBuffer[0]*3.0f;
    }
    
    if(freqChange){ //calculate alpha coefficients using SIMD
        
        ofx::f128 spread_v = ofx::m_load(freqMult);
        ofx::f128 alpha_v = ofx::m_mul1(spread_v, freqBuffer[0]); //now in alpha there are the cutoff freqs
        alpha_v = ofx::m_mul1(alpha_v, alphaMultiplier);
        alpha_v = ofx::m_tan(alpha_v);
        ofx::f128 alpha_num = ofx::m_sub1(alpha_v, 1.0f);
        ofx::f128 alpha_den = ofx::m_add1(alpha_v, 1.0f);
        alpha_den = ofx::m_rcp(alpha_den);
        alpha_v = ofx::m_mul(alpha_num, alpha_den);
        ofx::m_store(alpha, alpha_v);

    }   
    
    if(fbChange){
        fb = fbBuffer[0];
    } 
}

template<bool freqAR, bool fbAR, bool spreadAR>
void pdsp::APF4::process_audio(const float* inputBuffer, const float* freqBuffer, const float* fbBuffer, const float* spreadBuffer, const int &bufferSize)noexcept{
    
    float* outputBuffer = getOutputBufferToFill(output);
    
    for(int n=0; n<bufferSize; ++n){
    
        if(spreadAR){
            freqMult[0] = 1.0f;
            freqMult[1] = 1.0f + spreadBuffer[n];
            freqMult[2] = 1.0f + spreadBuffer[n]*2.0f;
            freqMult[3] = 1.0f + spreadBuffer[n]*3.0f;
        }
        
        if(freqAR){ //calculate alpha coefficients using SIMD
            ofx::f128 spread_v = ofx::m_load(freqMult);
            ofx::f128 alpha_v = ofx::m_mul1(spread_v, freqBuffer[n]); //now in alpha there are the cutoff freqs
            alpha_v = ofx::m_mul1(alpha_v, alphaMultiplier);
            alpha_v = ofx::m_tan(alpha_v);
            ofx::f128 alpha_num = ofx::m_sub1(alpha_v, 1.0f);
            ofx::f128 alpha_den = ofx::m_add1(alpha_v, 1.0f);
            alpha_den = ofx::m_rcp(alpha_den);
            alpha_v = ofx::m_mul(alpha_num, alpha_den);
            ofx::m_store(alpha, alpha_v);
            
        }
        
        if(fbAR){
            fb = fbBuffer[n];
        }
        
        float xn = inputBuffer[n] + fb*fb_z1;
        
        //first stage
        float yn = alpha[0] * (xn - y_z1_0) + x_z1_0;
        y_z1_0 = yn;
        x_z1_0 = xn;
        
        xn = yn;
        
        //second stage
        yn = alpha[1] * (xn - y_z1_1) + x_z1_1;
        y_z1_1 = yn;
        x_z1_1 = xn;
        
        xn = yn;        
        
        //third stage
        yn = alpha[2] * (xn - y_z1_2) + x_z1_2;
        y_z1_2 = yn;
        x_z1_2 = xn;
        
        xn = yn;    
                 
        //fourth stage
        yn = alpha[3] * (xn - y_z1_3) + x_z1_3;
        y_z1_3 = yn;
        x_z1_3 = xn;        
        
        fb_z1 = yn;
        outputBuffer[n] = yn;
        
    }
    
}











