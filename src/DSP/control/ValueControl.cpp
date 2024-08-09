
#include "ValueControl.h"

pdsp::ValueControl::ValueControl(){
    addOutput("signal", output);
    updateOutputNodes();
    slewInitValue = 0.0f;
    value = 0.0f;
    lastValue = std::numeric_limits<float>::infinity();
    this->deactivateSlew();
    
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
    
}

pdsp::ValueControl::ValueControl(const pdsp::ValueControl & other) : pdsp::ValueControl(){
    this->value.store ( other.value );
    this->slewInitValue = this->value;
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
}

pdsp::ValueControl& pdsp::ValueControl::operator=(const pdsp::ValueControl & other){
    this->value.store( other.value );
    this->slewInitValue = this->value;    
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
    return *this;
}

void pdsp::ValueControl::enableSmoothing(float timeMs){
    this->setSlewTime(timeMs);
    
}

void pdsp::ValueControl::disableSmoothing(){
    this->deactivateSlew();
}

void pdsp::ValueControl::prepareUnit( int expectedBufferSize, double sampleRate ) {      
    prepareSlew(sampleRate, value);   
}

void pdsp::ValueControl::releaseResources (){}


void pdsp::ValueControl::process (int bufferSize) noexcept {

    float newValue = value;
    
    if(newValue!=lastValue){
            valueChange(newValue);
    }
    if(slewRun){
        float* outputBuffer = getOutputBufferToFill(output);
        runSlewBlock(outputBuffer, 0, bufferSize);         
    }else{
        setControlRateOutput(output, slewLastValue);
    }
    
    lastValue = newValue;
}
