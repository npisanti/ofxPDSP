
#include "LFOPhazor.h"


pdsp::LFOPhazor::LFOPhazor(){
    
        phase = 0.0f;

        addInput("freq", input_freq);
        addInput("retrig", input_retrig);
        addInput("phase_start", input_phase_start);
        addOutput("phase", output_phase);
        addOutput("trig", output_trig);
        updateOutputNodes();

        input_freq.setDefaultValue(0.25f);
        input_phase_start.setDefaultValue(0.0f);

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::LFOPhazor::set(float freq, float phase_start){
        input_freq.setDefaultValue(freq);
        input_phase_start.setDefaultValue(phase_start); 
        return *this;   
}

pdsp::Patchable& pdsp::LFOPhazor::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::LFOPhazor::in_retrig(){
    return in("retrig");
}

pdsp::Patchable& pdsp::LFOPhazor::in_phase_start(){
    return in("phase_start");
}      

pdsp::Patchable& pdsp::LFOPhazor::out_phase(){
    return out("phase");
}

pdsp::Patchable& pdsp::LFOPhazor::out_trig(){
    return out("trig");
}

void pdsp::LFOPhazor::prepareUnit( int expectedBufferSize, double sampleRate) {
    phase = 0.0f;
    incCalculationMultiplier = 1.0f / sampleRate;
}

void pdsp::LFOPhazor::releaseResources () {
    
}

void pdsp::LFOPhazor::process (int bufferSize) noexcept {
        
        int freqBufferState;
        const float* freqBuffer = processInput(input_freq, freqBufferState);
        int trigBufferState;
        const float* trigBuffer = processInput(input_retrig, trigBufferState);

        if(freqBufferState!=AudioRate){
                inc = freqBuffer[0] * incCalculationMultiplier;
        }
        
        int switcher = freqBufferState + trigBufferState*4;
        //process audio rate
        switch ( switcher & processAudioBitMask ) {
        case audioFFFF :
                process_audio<false, false>(freqBuffer, trigBuffer, bufferSize);
                break;
        case audioFTFF :
                process_audio<false, true>(freqBuffer, trigBuffer, bufferSize);
                break;
        case audioTFFF :
                process_audio<true, false>(freqBuffer, trigBuffer, bufferSize);
                break;
        case audioTTFF :
                process_audio<true, true>(freqBuffer, trigBuffer, bufferSize);
                break;
        default:
                break;
        }
}

template<bool pitchAR, bool trigAR>
void pdsp::LFOPhazor::process_audio(const float* freqBuffer, const float* trigBuffer, int bufferSize)noexcept{
    
        float* outputBuffer = getOutputBufferToFill(output_phase);
        float* trigOutput = nullptr;

        if(pitchAR){
                ofx_Aeq_BmulS(outputBuffer, freqBuffer, incCalculationMultiplier, bufferSize);
        }

        for (int n=0; n<bufferSize; ++n) {

                if(pitchAR){
                        inc = outputBuffer[n];
                }

                if(trigAR){
                        if ( checkTrigger(trigBuffer[n]) ) {
                                phase = processAndGetSingleValue(input_phase_start, n);
                        }
                }

                if( phase>=1.0f ) {
                        onReset(n, inc, trigOutput, bufferSize);
                }

                outputBuffer[n] = phase;

                phase += inc;
        }

        if (trigOutput==nullptr) {
            setOutputToZero(output_trig);
        }
    
}



void pdsp::LFOPhazor::onReset(const int &n, const float &inc, float* &triggerBuffer, const int &bufferSize){
        phase-=1.0f;
        if(triggerBuffer==nullptr){
                triggerBuffer = getOutputBufferToFill(output_trig);
                ofx_Aeq_Zero(triggerBuffer, bufferSize);
        }
        triggerBuffer[n] = 1.0f; 
    
}
