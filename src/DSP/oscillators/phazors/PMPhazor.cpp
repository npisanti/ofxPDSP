

#include "PMPhazor.h"
//#include <iostream>

pdsp::PMPhazor::PMPhazor(){
    
        phase = 0.0f;

        addInput("freq", input_freq);
        addInput("sync", input_sync);
        addInput("pm", input_phase_mod);
        addOutput("phase", output_phase);
        addOutput("inc", output_inc);
        addOutput("sync", output_sync);
        updateOutputNodes();

        input_freq.setDefaultValue(440.0f);

        input_phase_mod.setDefaultValue(0.0f);

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::PMPhazor::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::PMPhazor::in_pm(){
    return in("pm");
}

pdsp::Patchable& pdsp::PMPhazor::in_sync(){
    return in("sync");
}

pdsp::Patchable& pdsp::PMPhazor::out_phase(){
    return out("phase");
}

pdsp::Patchable& pdsp::PMPhazor::out_inc(){
    return out("inc");
}

pdsp::Patchable& pdsp::PMPhazor::out_sync(){
    return out("sync");
}

void pdsp::PMPhazor::prepareUnit( int expectedBufferSize, double sampleRate) {

        phase = 0.0f;

        incCalculationMultiplier = 1.0f / sampleRate;
        
}

void pdsp::PMPhazor::releaseResources () {
    
}

void pdsp::PMPhazor::process (int bufferSize) noexcept {
        
        int freqBufferState;
        const float* freqBuffer = processInput(input_freq, freqBufferState);
        int syncBufferState;
        const float* syncBuffer = processInput(input_sync, syncBufferState);
        int phaseModState;
        const float* phaseModBuffer = processInput(input_phase_mod, phaseModState);
    
        if(freqBufferState!=AudioRate){
                inc = freqBuffer[0] * incCalculationMultiplier;
        }

        setControlRateOutput(output_inc, inc);

        int switcher = freqBufferState + syncBufferState*4 + phaseModState*16;
        //process audio rate
        switch ( switcher & processAudioBitMask ) {
        case audioFFFF :
                process_audio<false, false, false>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioFFTF :
                process_audio<false, false, true>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioFTFF :
                process_audio<false, true, false>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioFTTF:
                process_audio<false, true, true>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioTFFF :
                process_audio<true, false, false>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioTFTF :
                process_audio<true, false, true>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioTTFF :
                process_audio<true, true, false>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        case audioTTTF:
                process_audio<true, true, true>(freqBuffer, syncBuffer, phaseModBuffer, bufferSize);
                break;
        default:
                break;
        }

}

template<bool pitchAR, bool syncAR, bool phaseAR>
void pdsp::PMPhazor::process_audio(const float* freqBuffer, const float* syncBuffer, const float* pmBuffer, int bufferSize)noexcept{
    
        float* outputBuffer = getOutputBufferToFill(output_phase);
        float* incBuffer = nullptr;

        float* syncOutput = getOutputBufferToFill(output_sync);
        ofx_Aeq_S(syncOutput, pdspSyncNull, bufferSize);

        if(pitchAR){
                ofx_Aeq_BmulS(outputBuffer, freqBuffer, incCalculationMultiplier, bufferSize);
                incBuffer = getOutputBufferToFill(output_inc);
        }

        for (int n=0; n<bufferSize; ++n) {

                if(pitchAR){
                        inc = outputBuffer[n];
                }

                if(syncAR){
                        if ( checkSync(syncBuffer[n]) ) {
                                phase = syncBuffer[n]*inc; //right phase offset 
                        }
                }

                if( phase>=1.0f ) {
                        phase -= 1.0f;
                        syncOutput[n] = (inc-phase)/inc;	//intersample phase out
                }


                outputBuffer[n] = phase;

                phase += inc;

                if(pitchAR){
                        incBuffer[n] = inc;
                }
        }

        if(phaseAR){
                vect_phazorShiftB(outputBuffer, pmBuffer, bufferSize);
                phaseOffset = pmBuffer[bufferSize-1];
        }
    
}



