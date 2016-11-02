
#include "ofxPDSPValue.h"

ofxPDSPValue::ofxPDSPValue(){
    addOutput("signal", output);
    updateOutputNodes();
    slewInitValue = 0.0f;
    value = 0.0f;
    lastValue = numeric_limits<float>::infinity();
    this->deactivateSlew();
  
    parameter.addListener(this, &ofxPDSPValue::onSet);
    parameter_i.addListener(this, &ofxPDSPValue::onSetI);
    parameter.set( 0.0f );
    parameter_i.set( 0 );
    
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
    this->slewInitValue = this->value;    
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
    return *this;
}

ofxPDSPValue::~ofxPDSPValue(){
    parameter.removeListener(this, &ofxPDSPValue::onSet);
    parameter_i.removeListener(this, &ofxPDSPValue::onSetI);
}

ofParameter<float>& ofxPDSPValue::set(const char * name, float value, float min, float max){
    parameter.set(name, value, min, max);
    return parameter;
}

ofParameter<float>& ofxPDSPValue::set(const char * name, float min, float max){
    parameter.set(name, parameter.get(), min, max);
    return parameter;
}

ofParameter<int>& ofxPDSPValue::set(const char * name, int value, int min, int max){
    parameter_i.set(name, value, min, max);
    return parameter_i;
}

ofParameter<int>& ofxPDSPValue::set(const char * name, int min, int max){
    parameter.set(name, parameter_i.get(), min, max);
    return parameter_i;
}

ofParameter<float>& ofxPDSPValue::getOFParameterFloat(){
    return parameter;
}

ofParameter<int>& ofxPDSPValue::getOFParameterInt(){
    return parameter_i;
}

void ofxPDSPValue::setv(float value){
    this->value = value;
}


void ofxPDSPValue::onSet(float &newValue){
    //value = newValue;
    value = parameter.get() + static_cast<float>(parameter_i.get());
}

void ofxPDSPValue::onSetI(int  &newValue){
    //value = newValue;
    value = parameter.get() + static_cast<float>(parameter_i.get());
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


ofParameter<float>& ofxPDSPValue::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& ofxPDSPValue::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& ofxPDSPValue::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& ofxPDSPValue::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}
