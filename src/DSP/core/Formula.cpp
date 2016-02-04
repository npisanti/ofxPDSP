
#include "Formula.h"

pdsp::Formula::Formula(){
        addInput("signal", input);
        addOutput("signal", output);
        updateOutputNodes();
        lastProcessedValue = 0.0f;
        input.setDefaultValue(0.0f);

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::Formula::set(float value){
    input.setDefaultValue(value);
    return *this;
}

void pdsp::Formula::enableBoundaries( float low, float high){
    input.enableBoundaries(low, high);
}
                
void pdsp::Formula::disableBoundaries(){
    input.disableBoundaries();
}

float pdsp::Formula::meter_input() const{
    return inputMeter.load();
}

float pdsp::Formula::meter_output() const{
    return outputMeter.load();
}

void pdsp::Formula::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::Formula::releaseResources () {}

//function to be overloaded for faster vectorialized operations
void pdsp::Formula::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept{
        for(int n=0; n<bufferSize; ++n){
                output[n] = formula(input[n]);
        }
}

void pdsp::Formula::process (int bufferSize) noexcept {

        int inputState;
        const float* inputBuffer = processInput(input, inputState );
        
        switch(inputState){
        case Unchanged:
                setControlRateOutput(output, lastProcessedValue);
                break;
        case Changed:
                lastProcessedValue = formula(inputBuffer[0]);
                setControlRateOutput(output, lastProcessedValue);
                inputMeter.store(inputBuffer[0]);
                outputMeter.store(lastProcessedValue);
                break;
        case AudioRate:
                float* outputBuffer = getOutputBufferToFill(output);
                formulaAudioRate(outputBuffer, inputBuffer, bufferSize);
                lastProcessedValue = outputBuffer[bufferSize-1];
                inputMeter.store(inputBuffer[0]);
                outputMeter.store(outputBuffer[0]);
                break;
        }
}
