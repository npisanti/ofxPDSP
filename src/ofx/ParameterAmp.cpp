
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
    channels(1);
}

pdsp::ParameterAmp::~ParameterAmp(){
    channels(0);
}

void pdsp::ParameterAmp::channels( int size ){
    int oldsize = amps.size();
    if( size >= oldsize ){
        amps.resize( size );
        for (size_t i=oldsize; i<amps.size(); ++i ){
            amps[i] = new Amp();
            value >> amps[i]->in_mod();
            addModuleInput( std::to_string(i).c_str(), *amps[i] );
            addModuleOutput( std::to_string(i).c_str(), *amps[i] );
        }        
    }else{
        for (int i=size; i<oldsize; ++i ){
            delete amps[i];
        }
        amps.resize( size );
    }
}

pdsp::Patchable& pdsp::ParameterAmp::ini( int ch ){
    return in( std::to_string(ch).c_str() );
}

pdsp::Patchable& pdsp::ParameterAmp::outi( int ch ){
    return out( std::to_string(ch).c_str() );
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

float pdsp::ParameterAmp::meter_output( int ch ) const {
    return amps[ch]->meter_output();
}  

float pdsp::ParameterAmp::meter_mod() const {
    return value.meter_output();
}  

pdsp::Patchable& pdsp::ParameterAmp::in_signal() {
    return in("0");
}

pdsp::Patchable& pdsp::ParameterAmp::out_signal() {
    return out("0");
}

ofParameter<bool>& pdsp::ParameterAmp::set( std::string name, bool value, float min, float max ){
    return this->value.set( name, value, min, max );
}

ofParameter<bool>& pdsp::ParameterAmp::set( const char * name, bool value, float min, float max ){
    return this->value.set( name, value, min, max );
}
