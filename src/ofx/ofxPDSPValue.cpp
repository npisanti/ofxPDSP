
#include "ofxPDSPValue.h"

ofxPDSPValue::ofxPDSPValue(){

    addOutput("signal", output);
    updateOutputNodes();
    slewInitValue = 0.0f;
    value = 0.0f;
    lastValue = numeric_limits<float>::infinity();
    this->deactivateSlew();
    parameter.addListener(this, &ofxPDSPValue::onSet);
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
    
}

ofxPDSPValue::ofxPDSPValue(const ofxPDSPValue & other) : ofxPDSPValue(){
    this->parameter = other.parameter;
    this->value.store ( other.value );
    this->slewInitValue = this->value;
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
    
}

ofxPDSPValue& ofxPDSPValue::operator=(const ofxPDSPValue & other){
    this->parameter = other.parameter;
    this->value.store( other.value );
    return *this;
}


ofxPDSPValue::~ofxPDSPValue(){
    parameter.removeListener(this, &ofxPDSPValue::onSet);
}

ofParameter<float>& ofxPDSPValue::set(const char * name, float value, float min, float max){
    parameter.set(name, value, min, max);
    return parameter;
}

ofParameter<float>& ofxPDSPValue::set(const char * name, float min, float max){
    parameter.set(name, this->value, min, max);
    return parameter;
}

ofParameter<float>& ofxPDSPValue::getOFParameter(){
    return parameter;
}

void ofxPDSPValue::setv(float value){
    this->value = value;
}

pdsp::Patchable& ofxPDSPValue::setp(float value){
    parameter.set(value);
    return *this;
}

void ofxPDSPValue::onSet(float &newValue){
    value = newValue;
}

float ofxPDSPValue::get() const{
    return value;
}

void ofxPDSPValue::enableSmoothing(float timeMs){
    this->setSlewTime(timeMs);
    
}

void ofxPDSPValue::disableSmoothing(){
    this->deactivateSlew();
}

void ofxPDSPValue::prepareUnit( int expectedBufferSize, double sampleRate ) {      
    prepareSlew(sampleRate, value);   
}

void ofxPDSPValue::releaseResources (){}


void ofxPDSPValue::process (int bufferSize) noexcept {

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

float ofxPDSPValue::meter_output() const{
    return slewLastValue.load();
}  
