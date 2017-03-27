
#include "ofxPDSPStereoFader.h"

ofxPDSPStereoFader::ofxPDSPStereoFader(){
    patch();
}

ofxPDSPStereoFader::ofxPDSPStereoFader(const ofxPDSPStereoFader & other) : ofxPDSPStereoFader(){
    patch();
}

ofxPDSPStereoFader& ofxPDSPStereoFader::operator=(const ofxPDSPStereoFader & other){
    patch();
    return *this;
}
    
void ofxPDSPStereoFader::patch(){
    addModuleInput("0", amp0);
    addModuleInput("1", amp1);
    addModuleOutput("0", amp0);
    addModuleOutput("1", amp1);
    value >> dBtoLin >> amp0.in_mod();
             dBtoLin >> amp1.in_mod();
}

ofParameter<float>& ofxPDSPStereoFader::set(const char * name, float value, float min, float max) {
    return this->value.set( name, value, min, max );    
}

ofParameter<int>& ofxPDSPStereoFader::set(const char * name, int value, int min, int max) {
    return this->value.set( name, value, min, max );
}

ofParameter<float>& ofxPDSPStereoFader::set(const char * name, float min, float max) {
    return this->value.set(name, min, max);
}

ofParameter<int>& ofxPDSPStereoFader::set(const char * name, int min, int max) {
    return this->value.set(name, min, max);
}
 
ofParameter<float>& ofxPDSPStereoFader::set( std::string name, float value, float min, float max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<int>& ofxPDSPStereoFader::set( std::string name, int value, int min, int max) {
    return set( name.c_str(), value, min, max);
}

ofParameter<float>& ofxPDSPStereoFader::set( std::string name, float min, float max){
    return set( name.c_str(), min, max);
}

ofParameter<int>& ofxPDSPStereoFader::set( std::string name, int min, int max) {
    return set( name.c_str(), min, max);
}

void ofxPDSPStereoFader::setv(float value) {
    this->value.setv(value);
}

ofParameter<float>& ofxPDSPStereoFader::getOFParameterFloat() {
    return value.getOFParameterFloat();
}

ofParameter<int>& ofxPDSPStereoFader::getOFParameterInt() {
    return value.getOFParameterInt();
}

void ofxPDSPStereoFader::enableSmoothing(float timeMs) {
    value.enableSmoothing( timeMs );
}

void ofxPDSPStereoFader::disableSmoothing() {
    value.disableSmoothing();
}


float ofxPDSPStereoFader::meter_0() const {
    return amp0.meter_output();
}  

float ofxPDSPStereoFader::meter_1() const {
    return amp1.meter_output();
}  

float ofxPDSPStereoFader::meter_L() const {
    return amp0.meter_output();
}  

float ofxPDSPStereoFader::meter_R() const {
    return amp1.meter_output();
}  

float ofxPDSPStereoFader::meter_mod() const {
    return value.meter_output();
}  


pdsp::Patchable& ofxPDSPStereoFader::in_0() {
    return in("0");
}

pdsp::Patchable& ofxPDSPStereoFader::in_1() {
    return in("1");
}

pdsp::Patchable& ofxPDSPStereoFader::in_L() {
    return in("0");
}

pdsp::Patchable& ofxPDSPStereoFader::in_R() {
    return in("1");
}

pdsp::Patchable& ofxPDSPStereoFader::out_0() {
    return out("0");
}

pdsp::Patchable& ofxPDSPStereoFader::out_1() {
    return out("1");
}

pdsp::Patchable& ofxPDSPStereoFader::out_L() {
    return out("0");
}

pdsp::Patchable& ofxPDSPStereoFader::out_R() {
    return out("1");
}

float ofxPDSPStereoFader::get() const{
    return value.get();
}
