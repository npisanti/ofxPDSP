

#include "Bitcruncher.h"



pdsp::Bitcruncher::Bitcruncher(){
    
        addInput("signal", input_signal);
        addInput("bits", input_bits);
        addOutput("signal", output);
        updateOutputNodes();
        
}


pdsp::Patchable& pdsp::Bitcruncher::in_bits(){
    return in("bits");
}

pdsp::Patchable& pdsp::Bitcruncher::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Bitcruncher::out_signal(){
    return out("signal");
}

void pdsp::Bitcruncher::prepareUnit( int expectedBufferSize, double sampleRate ){   };

void pdsp::Bitcruncher::releaseResources () {   };

void pdsp::Bitcruncher::process (int bufferSize) noexcept  {
       
        int inputBufferState;
        const float* inputBuffer = processInput(input_signal, inputBufferState);
     
        if( inputBufferState == AudioRate){
            int bitsState;
            const float* bitsBuffer = processInput(input_bits, bitsState);
         
            switch (bitsState){
                case Unchanged:
                    process_audio<false>(inputBuffer, bitsBuffer, bufferSize);
                    break;
                case Changed:
                    multiply = powf(2.0f, bitsBuffer[0]);
                    scaleBack = 1.0f / multiply;
                    process_audio<false>(inputBuffer, bitsBuffer, bufferSize);
                    break;      
                case AudioRate:
                    process_audio<true>(inputBuffer, bitsBuffer, bufferSize);
                    break;          
            }     
        }else{
            setOutputToZero(output);
        }
}

template<bool bitsAR>
void pdsp::Bitcruncher::process_audio(const float* &inputBuffer, const float* &bitsBuffer, const int &bufferSize)noexcept {

    float* outputBuffer = getOutputBufferToFill( output );
    
    ofx::f128 mult0;
    ofx::f128 scale0;
    ofx::f128 mult4;
    ofx::f128 scale4;
    
    if(!bitsAR){
        mult0 = ofx::m_set1(multiply);
        scale0 = ofx::m_set1(scaleBack);
        mult4 = ofx::m_set1(multiply);
        scale4 = ofx::m_set1(scaleBack);
    }
    
    int n=0;
    for(; n<bufferSize; n+=8){
        
        if(bitsAR){
            mult0 = ofx::m_load( bitsBuffer+n );
            mult4 = ofx::m_load( bitsBuffer+n+4 );
            mult0 = ofx::m_pow2(mult0);
            mult4 = ofx::m_pow2(mult4);
            scale0 = ofx::m_rcp( mult0 );
            scale4 = ofx::m_rcp( mult4 );
        }
        
        ofx::f128 x0 = ofx::m_load( inputBuffer + n );
        ofx::f128 x4 = ofx::m_load( inputBuffer + n + 4 );
        x0 = ofx::m_mul(x0, mult0);
        x4 = ofx::m_mul(x4, mult4);
        x0 = ofx::m_trunc(x0);
        x4 = ofx::m_trunc(x4);
        x0 = ofx::m_mul(x0, scale0);
        x4 = ofx::m_mul(x4, scale4);
        
        ofx::m_store(outputBuffer + n,    x0);
        ofx::m_store(outputBuffer + n +4, x4);
    }
    
    
    for (; n<bufferSize; ++n) {
        
        if(bitsAR){
            multiply = powf(2.0f, bitsBuffer[n]);
            scaleBack = 1.0f / multiply;
        }
        
        float yn  = inputBuffer[n] * multiply;
        yn = static_cast<int> (yn);
        yn *= scaleBack;
        outputBuffer[n] = yn;
    }
    
 
}
