
#include "CRSlew.h"

pdsp::CRSlew::CRSlew(){
    addInput("signal", input);
    addOutput("signal", output);
    updateOutputNodes();
    slewInitValue = 0.0f;
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
}


pdsp::Patchable& pdsp::CRSlew::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::CRSlew::out_signal(){
    return out("signal");   
}

                
pdsp::Patchable& pdsp::CRSlew::set(float slewTime){
    setSlewTime(slewTime);
    return *this;
}

pdsp::Patchable& pdsp::CRSlew::set(float slewTime, float initValue){
    setSlewTime(slewTime);
    setInitValue(initValue);
    return *this;
}

void pdsp::CRSlew::prepareUnit( int expectedBufferSize, double sampleRate ) {      
       prepareSlew(sampleRate, slewInitValue);   
       meter.store(slewInitValue);    
}

void pdsp::CRSlew::releaseResources (){}


void pdsp::CRSlew::process (int bufferSize) noexcept {

    int inputChanged;
    const float* inputBuffer = processInput(input, inputChanged);

    if(inputChanged){
            valueChange(inputBuffer[0]);
            float* outputBuffer = getOutputBufferToFill(output);
            runSlewBlock(outputBuffer, 0, bufferSize);
    }else{
        if(slewRun){
            float* outputBuffer = getOutputBufferToFill(output);
            runSlewBlock(outputBuffer, 0, bufferSize);         
        }else{
            setControlRateOutput(output, slewLastValue);
        }
    }
    meter.store(slewLastValue);
}


float pdsp::CRSlew::meter_output() const {
    return meter.load();
}
