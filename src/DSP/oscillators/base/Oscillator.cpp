
#include "Oscillator.h"


pdsp::Oscillator::Oscillator(){
        addInput("phase", input_phase);
        addOutput("signal", output);
        updateOutputNodes();
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

float pdsp::Oscillator::meter_output() const{
    return meter.load();
}

pdsp::Patchable& pdsp::Oscillator::in_phase(){
        return in("phase");
}

pdsp::Patchable& pdsp::Oscillator::out_signal(){
        return out("signal");
}

void pdsp::Oscillator::prepareOscillator(double sampleRate){}
void pdsp::Oscillator::releaseOscillator(){}
void pdsp::Oscillator::oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept {}

void pdsp::Oscillator::prepareUnit( int expectedBufferSize, double sampleRate) {
        prepareOscillator( sampleRate );
}

void pdsp::Oscillator::releaseResources() {
        releaseOscillator();
}

void pdsp::Oscillator::process(int bufferSize) noexcept {

        int phaseBufferState;
        const float* phaseBuffer = processInput(input_phase, phaseBufferState);

        if (phaseBufferState == AudioRate){
                float* outputBuffer = getOutputBufferToFill(output);

                //----------------oscillator DSP-----------------------
                oscillate(outputBuffer, phaseBuffer, bufferSize);

                meter.store(outputBuffer[0]);
        }else{
                setOutputToZero(output);
                meter.store(0.0f);
        }

}
