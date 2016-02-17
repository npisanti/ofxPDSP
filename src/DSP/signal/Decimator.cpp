

#include "Decimator.h"
//#include <iostream>

pdsp::Decimator::Decimator(){
    
    phase = 1.0f;
    addInput("signal", input_signal);
    addInput("freq", input_freq);
    addOutput("output", output);
    updateOutputNodes();

    input_freq.setDefaultValue(44100.0f);

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::Decimator::set(float freq){
    input_freq.setDefaultValue(freq);
    return *this;   
}

pdsp::Patchable& pdsp::Decimator::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Decimator::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::Decimator::out_signal(){
    return out("signal");
}

void pdsp::Decimator::prepareUnit( int expectedBufferSize, double sampleRate) {
    phase = 1.0f;
    incCalculationMultiplier = 1.0f / sampleRate;
}

void pdsp::Decimator::releaseResources () {
    
}

void pdsp::Decimator::process (int bufferSize) noexcept {
        int inputBufferState;
        const float* inputBuffer = processInput(input_signal, inputBufferState);
          
        if( inputBufferState == AudioRate){
            int freqBufferState;
            const float* freqBuffer = processInput(input_freq, freqBufferState);
             
            switch (freqBufferState){
                case Unchanged:
                    process_audio<false>(inputBuffer, freqBuffer, bufferSize);
                    break;
                case Changed:
                    inc = freqBuffer[0] * incCalculationMultiplier;
                    process_audio<false>(inputBuffer, freqBuffer, bufferSize);
                    break;      
                case AudioRate:
                    process_audio<true>(inputBuffer, freqBuffer, bufferSize);
                    break;          
            }     
            
            if(phase >= 1.0f){ // if the fre rate is higher than the sampling rate this could happen
                int phase_i = static_cast<int> (phase);
                phase -= phase_i;
            }
        }else{
            setOutputToZero(output);
        }

}

template<bool pitchAR>
void pdsp::Decimator::process_audio(const float* inputBuffer, const float* freqBuffer, int bufferSize)noexcept{
    
    float* outputBuffer = getOutputBufferToFill(output);

    if(pitchAR){
        ofx_Aeq_BmulS(outputBuffer, freqBuffer, incCalculationMultiplier, bufferSize);
    }

    for (int n=0; n<bufferSize; ++n) {

        if(pitchAR){
            inc = outputBuffer[n];
        }

        if( phase>=1.0f ) {
            phase -= 1.0f;
            sampled = inputBuffer[n];
        }

        outputBuffer[n] = sampled;

        phase += inc;
    }

}




