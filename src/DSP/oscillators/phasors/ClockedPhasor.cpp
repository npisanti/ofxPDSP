
#include "ClockedPhasor.h"



pdsp::ClockedPhasor::ClockedPhasor(){

        addInput("retrig", input_trig);
        addOutput("phase", output_phase);
        addOutput("trig", output_trig);
        addInput("phase start", input_phase);
        addInput("division", input_division);
        updateOutputNodes();
        
        input_division.setDefaultValue(4.0f);


        input_phase.setDefaultValue(0.0f);
        oversampleFactor = 1.0f;

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}


pdsp::Patchable& pdsp::ClockedPhasor::set(float division, float phaseOffset){
    input_division.setDefaultValue(division);
    input_phase.setDefaultValue(phaseOffset);
    return *this;
}

pdsp::Patchable& pdsp::ClockedPhasor::in_retrig(){
    return in("retrig");
}

pdsp::Patchable& pdsp::ClockedPhasor::in_phase_offset(){
    return in("phase start");
}

pdsp::Patchable& pdsp::ClockedPhasor::in_division(){
    return in("division");
}

pdsp::Patchable& pdsp::ClockedPhasor::out_phase(){
    return out("phase");
}

pdsp::Patchable& pdsp::ClockedPhasor::out_trig(){
    return out("trig");
}

void pdsp::ClockedPhasor::prepareUnit( int expectedBufferSize, double sampleRate ){
        oversampleFactor = 1.0f / static_cast<float>(getOversampleLevel());
        phase = 0.0f;
}

void pdsp::ClockedPhasor::releaseResources(){}


void pdsp::ClockedPhasor::process(int bufferSize) noexcept {
        phaseOffset = processAndGetSingleValue(input_phase, 0);
        division = processAndGetSingleValue(input_division, 0);
   
        if (input_trig.isConnected()) {
                processTrigger(bufferSize);
        }else{
                processSync(bufferSize);
        }
}

void pdsp::ClockedPhasor::processSync(int bufferSize) noexcept {

        if( isPlaying() ){
                float temp = getGlobalBarPosition() / division;
                int temp_i = static_cast<int>(temp);
                phase = temp - temp_i; //phase is the fractional part
                phase += phaseOffset;
                if(phase>=1.0f) { phase -= 1.0f; }
        }


        float* outPhaseBuffer = getOutputBufferToFill(output_phase);
        float* outTrigBuffer = nullptr;

        float inc = getBarsPerSample() * division * oversampleFactor;

        for (int n=0; n<bufferSize; ++n) {
                if (phase >= 1.0f) {
                        onReset(n, inc, outTrigBuffer, bufferSize);
                }
                outPhaseBuffer[n] = phase;
                phase += inc;
        }

        if (outTrigBuffer==nullptr) {
                setOutputToZero(output_trig);
        }

}

void pdsp::ClockedPhasor::processTrigger(int bufferSize) noexcept {

        int triggerState;
        const float* inTrigBuffer = processInput(input_trig, triggerState);

        float* outPhaseBuffer = getOutputBufferToFill(output_phase);
        float* outTrigBuffer = nullptr;

        float inc = getBarsPerSample() * division * oversampleFactor;

        switch (triggerState) {
        case AudioRate:
                for( int n=0; n<bufferSize; ++n){
                        if(checkTrigger( inTrigBuffer[n] )){
                                phase = phaseOffset;
                        }
                        else if (phase>=1.0f) {
                                onReset(n, inc, outTrigBuffer, bufferSize);
                        }
                        outPhaseBuffer[n] = phase;
                        phase += inc;
                }
                break;
        default:
                for( int n=0; n<bufferSize; ++n){
                        if (phase>=1.0f) {
                                onReset(n, inc, outTrigBuffer, bufferSize);
                        }
                        outPhaseBuffer[n] = phase;
                        phase += inc;
                }
                break;
        }

        if (outTrigBuffer==nullptr) {
                setOutputToZero(output_trig);
        }
   
}


void pdsp::ClockedPhasor::onReset(const int &n, const float &inc, float* &triggerBuffer, const int &bufferSize){
        phase-=1.0f;
        if(triggerBuffer==nullptr){
                triggerBuffer = getOutputBufferToFill(output_trig);
                ofx_Aeq_Zero(triggerBuffer, bufferSize);
        }
        triggerBuffer[n] = 1.0f;  //inter sample trigger value
    
}
