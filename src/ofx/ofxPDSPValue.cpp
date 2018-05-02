
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
    parameter_b.addListener(this, &ofxPDSPValue::onSetB);
    parameter.set( 0.0f );
    parameter_i.set( 0 );
    
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
    
}

ofxPDSPValue::ofxPDSPValue(const ofxPDSPValue & other) : ofxPDSPValue(){
    this->parameter = other.parameter;
    this->parameter_i = other.parameter_i;
    this->parameter_b = other.parameter_b;
    this->value.store ( other.value );
    this->slewInitValue = this->value;
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
}

ofxPDSPValue& ofxPDSPValue::operator=(const ofxPDSPValue & other){
    this->parameter = other.parameter;
    this->parameter_i = other.parameter_i;
    this->parameter_b = other.parameter_b;
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
    parameter_b.removeListener(this, &ofxPDSPValue::onSetB);
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

ofParameter<bool>& ofxPDSPValue::set(const char * name, bool value ){
    parameter_b.set( name, value );
    return parameter_b;
}

ofParameter<float>& ofxPDSPValue::getOFParameterFloat(){
    return parameter;
}

ofParameter<int>& ofxPDSPValue::getOFParameterInt(){
    return parameter_i;
}

ofParameter<bool>& ofxPDSPValue::getOFParameterBool(){
    return parameter_b;
}

void ofxPDSPValue::setv(float value){
    this->value = value;
}

void ofxPDSPValue::onSet(float &newValue){
    value = parameter.get() + static_cast<float>(parameter_i.get());
}

void ofxPDSPValue::onSetI(int  &newValue){
    value = parameter.get() + static_cast<float>(parameter_i.get());
}

void ofxPDSPValue::onSetB(bool  &newValue){
    if( newValue ){
        value = 1.0f;
    }else{
        value = 0.0f;
    }
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

ofParameter<bool>& ofxPDSPValue::set( std::string name, bool value ) {
    return set( name.c_str(), value );
}
