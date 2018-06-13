
#include "Parameter.h"

pdsp::Parameter::Parameter(){
    addOutput("signal", output);
    updateOutputNodes();
    slewInitValue = 0.0f;
    value = 0.0f;
    lastValue = numeric_limits<float>::infinity();
    this->deactivateSlew();
  
    parameter.addListener(this, &pdsp::Parameter::onSet);
    parameter_i.addListener(this, &pdsp::Parameter::onSetI);
    parameter_b.addListener(this, &pdsp::Parameter::onSetB);
    parameter.set( 0.0f );
    parameter_i.set( 0 );
    
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
    
}

pdsp::Parameter::Parameter(const pdsp::Parameter & other) : pdsp::Parameter(){
    this->parameter = other.parameter;
    this->parameter_i = other.parameter_i;
    this->parameter_b = other.parameter_b;
    this->value.store ( other.value );
    this->slewInitValue = this->value;
    if(dynamicConstruction){
            prepareSlew(sampleRate, slewInitValue);   
    }
}

pdsp::Parameter& pdsp::Parameter::operator=(const pdsp::Parameter & other){
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

pdsp::Parameter::~Parameter(){
    parameter.removeListener(this, &pdsp::Parameter::onSet);
    parameter_i.removeListener(this, &pdsp::Parameter::onSetI);
    parameter_b.removeListener(this, &pdsp::Parameter::onSetB);
}

ofParameter<float>& pdsp::Parameter::set(const char * name, float value, float min, float max){
    parameter.set(name, value, min, max);
    return parameter;
}

ofParameter<float>& pdsp::Parameter::set(const char * name, float min, float max){
    parameter.set(name, parameter.get(), min, max);
    return parameter;
}

ofParameter<int>& pdsp::Parameter::set(const char * name, int value, int min, int max){
    parameter_i.set(name, value, min, max);
    return parameter_i;
}

ofParameter<int>& pdsp::Parameter::set(const char * name, int min, int max){
    parameter.set(name, parameter_i.get(), min, max);
    return parameter_i;
}

ofParameter<bool>& pdsp::Parameter::set(const char * name, bool value ){
    parameter_b.set( name, value );
    return parameter_b;
}

ofParameter<float>& pdsp::Parameter::getOFParameterFloat(){
    return parameter;
}

ofParameter<int>& pdsp::Parameter::getOFParameterInt(){
    return parameter_i;
}

ofParameter<bool>& pdsp::Parameter::getOFParameterBool(){
    return parameter_b;
}

void pdsp::Parameter::setv(float value){
    this->value = value;
}

void pdsp::Parameter::onSet(float &newValue){
    value = parameter.get() + static_cast<float>(parameter_i.get());
}

void pdsp::Parameter::onSetI(int  &newValue){
    value = parameter.get() + static_cast<float>(parameter_i.get());
}

void pdsp::Parameter::onSetB(bool  &newValue){
    if( newValue ){
        value = 1.0f;
    }else{
        value = 0.0f;
    }
}

float pdsp::Parameter::get() const{
    return value;
}

void pdsp::Parameter::enableSmoothing(float timeMs){
    this->setSlewTime(timeMs);
    
}

void pdsp::Parameter::disableSmoothing(){
    this->deactivateSlew();
}

void pdsp::Parameter::prepareUnit( int expectedBufferSize, double sampleRate ) {      
    prepareSlew(sampleRate, value);   
}

void pdsp::Parameter::releaseResources (){}


void pdsp::Parameter::process (int bufferSize) noexcept {

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

float pdsp::Parameter::meter_output() const{
    return slewLastValue.load();
}  


ofParameter<float>& pdsp::Parameter::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& pdsp::Parameter::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& pdsp::Parameter::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& pdsp::Parameter::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

ofParameter<bool>& pdsp::Parameter::set( std::string name, bool value ) {
    return set( name.c_str(), value );
}
