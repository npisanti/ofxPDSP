
#include "ofxPDSPMonoFader.h"

ofxPDSPMonoFader::ofxPDSPMonoFader(){
    patch();
}

ofxPDSPMonoFader::ofxPDSPMonoFader(const ofxPDSPMonoFader & other) : ofxPDSPMonoFader(){
    patch();
}

ofxPDSPMonoFader& ofxPDSPMonoFader::operator=(const ofxPDSPMonoFader & other){
    patch();
    return *this;
}
    
void ofxPDSPMonoFader::patch(){
    addModuleInput("signal", amp);
    addModuleOutput("signal", amp);
    value >> dBtoLin >> amp.in_mod();
}

ofParameter<float>& ofxPDSPMonoFader::set(const char * name, float value, float min, float max) {
    return this->value.set( name, value, min, max );    
}

ofParameter<int>& ofxPDSPMonoFader::set(const char * name, int value, int min, int max) {
    return this->value.set( name, value, min, max );
}

ofParameter<float>& ofxPDSPMonoFader::set(const char * name, float min, float max) {
    return this->value.set(name, min, max);
}

ofParameter<int>& ofxPDSPMonoFader::set(const char * name, int min, int max) {
    return this->value.set(name, min, max);
}
 
ofParameter<float>& ofxPDSPMonoFader::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& ofxPDSPMonoFader::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& ofxPDSPMonoFader::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& ofxPDSPMonoFader::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

void ofxPDSPMonoFader::setv(float value) {
    this->value.setv(value);
}

ofParameter<float>& ofxPDSPMonoFader::getOFParameterFloat() {
    return value.getOFParameterFloat();
}

ofParameter<int>& ofxPDSPMonoFader::getOFParameterInt() {
    return value.getOFParameterInt();
}

void ofxPDSPMonoFader::enableSmoothing(float timeMs) {
    value.enableSmoothing( timeMs );
}

void ofxPDSPMonoFader::disableSmoothing() {
    value.disableSmoothing();
}

float ofxPDSPMonoFader::meter_output() const {
    return amp.meter_output();
}  

float ofxPDSPMonoFader::meter_mod() const {
    return value.meter_output();
}  

pdsp::Patchable& ofxPDSPMonoFader::in_signal() {
    return in("signal");
}

pdsp::Patchable& ofxPDSPMonoFader::out_signal() {
    return out("signal");
}

float ofxPDSPMonoFader::get() const {
	return value.get();
}
