
#include "ParameterGain.h"

pdsp::ParameterGain::ParameterGain(){
    patch();
}

pdsp::ParameterGain::ParameterGain(const pdsp::ParameterGain & other) : pdsp::ParameterGain(){
    patch();
}

pdsp::ParameterGain& pdsp::ParameterGain::operator=(const pdsp::ParameterGain & other){
    patch();
    return *this;
}
    
void pdsp::ParameterGain::patch(){
    addModuleInput("0", amp0);
    addModuleInput("1", amp1);
    addModuleOutput("0", amp0);
    addModuleOutput("1", amp1);
    value >> dBtoLin >> amp0.in_mod();
             dBtoLin >> amp1.in_mod();
}

ofParameter<float>& pdsp::ParameterGain::set(const char * name, float value, float min, float max) {
    return this->value.set( name, value, min, max );    
}

ofParameter<int>& pdsp::ParameterGain::set(const char * name, int value, int min, int max) {
    return this->value.set( name, value, min, max );
}

ofParameter<float>& pdsp::ParameterGain::set(const char * name, float min, float max) {
    return this->value.set(name, min, max);
}

ofParameter<int>& pdsp::ParameterGain::set(const char * name, int min, int max) {
    return this->value.set(name, min, max);
}
 
ofParameter<float>& pdsp::ParameterGain::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& pdsp::ParameterGain::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& pdsp::ParameterGain::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& pdsp::ParameterGain::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

void pdsp::ParameterGain::setv(float value) {
    this->value.setv(value);
}

ofParameter<float>& pdsp::ParameterGain::getOFParameterFloat() {
    return value.getOFParameterFloat();
}

ofParameter<int>& pdsp::ParameterGain::getOFParameterInt() {
    return value.getOFParameterInt();
}

void pdsp::ParameterGain::enableSmoothing(float timeMs) {
    value.enableSmoothing( timeMs );
}

void pdsp::ParameterGain::disableSmoothing() {
    value.disableSmoothing();
}


float pdsp::ParameterGain::meter_0() const {
    return amp0.meter_output();
}  

float pdsp::ParameterGain::meter_1() const {
    return amp1.meter_output();
}  

float pdsp::ParameterGain::meter_L() const {
    return amp0.meter_output();
}  

float pdsp::ParameterGain::meter_R() const {
    return amp1.meter_output();
}  

float pdsp::ParameterGain::meter_mod() const {
    return value.meter_output();
}  


pdsp::Patchable& pdsp::ParameterGain::in_0() {
    return in("0");
}

pdsp::Patchable& pdsp::ParameterGain::in_1() {
    return in("1");
}

pdsp::Patchable& pdsp::ParameterGain::in_L() {
    return in("0");
}

pdsp::Patchable& pdsp::ParameterGain::in_R() {
    return in("1");
}

pdsp::Patchable& pdsp::ParameterGain::out_0() {
    return out("0");
}

pdsp::Patchable& pdsp::ParameterGain::out_1() {
    return out("1");
}

pdsp::Patchable& pdsp::ParameterGain::out_L() {
    return out("0");
}

pdsp::Patchable& pdsp::ParameterGain::out_R() {
    return out("1");
}

float pdsp::ParameterGain::get() const{
    return value.get();
}
