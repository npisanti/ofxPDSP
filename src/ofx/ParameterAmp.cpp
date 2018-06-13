
#include "ParameterAmp.h"

pdsp::ParameterAmp::ParameterAmp(){
    patch();
}

pdsp::ParameterAmp::ParameterAmp(const pdsp::ParameterAmp & other) : pdsp::ParameterAmp(){
    patch();
}

pdsp::ParameterAmp& pdsp::ParameterAmp::operator=(const pdsp::ParameterAmp & other){
    patch();
    return *this;
}
    
void pdsp::ParameterAmp::patch(){
    addModuleInput("signal", amp);
    addModuleOutput("signal", amp);
    value >> amp.in_mod();
}

ofParameter<float>& pdsp::ParameterAmp::set(const char * name, float value, float min, float max) {
    return this->value.set( name, value, min, max );    
}

ofParameter<int>& pdsp::ParameterAmp::set(const char * name, int value, int min, int max) {
    return this->value.set( name, value, min, max );
}

ofParameter<float>& pdsp::ParameterAmp::set(const char * name, float min, float max) {
    return this->value.set(name, min, max);
}

ofParameter<int>& pdsp::ParameterAmp::set(const char * name, int min, int max) {
    return this->value.set(name, min, max);
}
 
ofParameter<float>& pdsp::ParameterAmp::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& pdsp::ParameterAmp::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& pdsp::ParameterAmp::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& pdsp::ParameterAmp::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

void pdsp::ParameterAmp::setv(float value) {
    this->value.setv(value);
}

ofParameter<float>& pdsp::ParameterAmp::getOFParameterFloat() {
    return value.getOFParameterFloat();
}

ofParameter<int>& pdsp::ParameterAmp::getOFParameterInt() {
    return value.getOFParameterInt();
}

void pdsp::ParameterAmp::enableSmoothing(float timeMs) {
    value.enableSmoothing( timeMs );
}

void pdsp::ParameterAmp::disableSmoothing() {
    value.disableSmoothing();
}

float pdsp::ParameterAmp::meter_output() const {
    return amp.meter_output();
}  

float pdsp::ParameterAmp::meter_mod() const {
    return value.meter_output();
}  

pdsp::Patchable& pdsp::ParameterAmp::in_signal() {
    return in("signal");
}

pdsp::Patchable& pdsp::ParameterAmp::out_signal() {
    return out("signal");
}

