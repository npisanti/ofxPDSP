
#include "ofxPDSPValueMultiplier.h"

ofxPDSPValueMultiplier::ofxPDSPValueMultiplier(){
    patch();
}

ofxPDSPValueMultiplier::ofxPDSPValueMultiplier(const ofxPDSPValueMultiplier & other) : ofxPDSPValueMultiplier(){
    patch();
}

ofxPDSPValueMultiplier& ofxPDSPValueMultiplier::operator=(const ofxPDSPValueMultiplier & other){
    patch();
    return *this;
}
    
void ofxPDSPValueMultiplier::patch(){
    addModuleInput("signal", amp);
    addModuleOutput("signal", amp);
    value >> amp.in_mod();
}

ofParameter<float>& ofxPDSPValueMultiplier::set(const char * name, float value, float min, float max) {
    return this->value.set( name, value, min, max );    
}

ofParameter<int>& ofxPDSPValueMultiplier::set(const char * name, int value, int min, int max) {
    return this->value.set( name, value, min, max );
}

ofParameter<float>& ofxPDSPValueMultiplier::set(const char * name, float min, float max) {
    return this->value.set(name, min, max);
}

ofParameter<int>& ofxPDSPValueMultiplier::set(const char * name, int min, int max) {
    return this->value.set(name, min, max);
}
 
ofParameter<float>& ofxPDSPValueMultiplier::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& ofxPDSPValueMultiplier::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& ofxPDSPValueMultiplier::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& ofxPDSPValueMultiplier::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

void ofxPDSPValueMultiplier::setv(float value) {
    this->value.setv(value);
}

ofParameter<float>& ofxPDSPValueMultiplier::getOFParameterFloat() {
    return value.getOFParameterFloat();
}

ofParameter<int>& ofxPDSPValueMultiplier::getOFParameterInt() {
    return value.getOFParameterInt();
}

void ofxPDSPValueMultiplier::enableSmoothing(float timeMs) {
    value.enableSmoothing( timeMs );
}

void ofxPDSPValueMultiplier::disableSmoothing() {
    value.disableSmoothing();
}

float ofxPDSPValueMultiplier::meter_output() const {
    return amp.meter_output();
}  

float ofxPDSPValueMultiplier::meter_mod() const {
    return value.meter_output();
}  

pdsp::Patchable& ofxPDSPValueMultiplier::in_signal() {
    return in("signal");
}

pdsp::Patchable& ofxPDSPValueMultiplier::out_signal() {
    return out("signal");
}

