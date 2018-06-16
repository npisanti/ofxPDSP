
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
    
pdsp::ParameterGain::~ParameterGain(){
    channels(0);
}
    
void pdsp::ParameterGain::patch(){
    channels(1);
    value >> dBtoLin;
}

void pdsp::ParameterGain::channels( int size ){
    int oldsize = amps.size();
    if( size >= oldsize ){
        amps.resize( size );
        for (size_t i=oldsize; i<amps.size(); ++i ){
            amps[i] = new Amp();
            dBtoLin >> amps[i]->in_mod();
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

pdsp::Patchable& pdsp::ParameterGain::operator[]( const int & ch ){
    return *(amps[ch]);
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
    return amps[0]->meter_output();
}  

float pdsp::ParameterGain::meter_1() const {
    return amps[1]->meter_output();
}  

float pdsp::ParameterGain::meter_L() const {
    return amps[0]->meter_output();
}  

float pdsp::ParameterGain::meter_R() const {
    return amps[1]->meter_output();
}  

float pdsp::ParameterGain::meter_mod() const {
    return value.meter_output();
}  

pdsp::Patchable& pdsp::ParameterGain::in_0() {
    if( amps.size()<2 ) channels(2);
    return in("0");
}

pdsp::Patchable& pdsp::ParameterGain::in_1() {
    if( amps.size()<2 ) channels(2);
    return in("1");
}

pdsp::Patchable& pdsp::ParameterGain::in_L() {
    if( amps.size()<2 ) channels(2);
    return in("0");
}

pdsp::Patchable& pdsp::ParameterGain::in_R() {
    if( amps.size()<2 ) channels(2);
    return in("1");
}

pdsp::Patchable& pdsp::ParameterGain::out_0() {
    if( amps.size()<2 ) channels(2);
    return out("0");
}

pdsp::Patchable& pdsp::ParameterGain::out_1() {
    if( amps.size()<2 ) channels(2);
    return out("1");
}

pdsp::Patchable& pdsp::ParameterGain::out_L() {
    if( amps.size()<2 ) channels(2);
    return out("0");
}

pdsp::Patchable& pdsp::ParameterGain::out_R() {
    if( amps.size()<2 ) channels(2);
    return out("1");
}

float pdsp::ParameterGain::get() const{
    return value.get();
}

float pdsp::ParameterGain::meter_output( int ch ) const {
    return amps[ch]->meter_output();
}  

ofParameter<bool>& pdsp::ParameterGain::set( std::string name, bool value, float min, float max ){
    return this->value.set( name, value, min, max );
}

ofParameter<bool>& pdsp::ParameterGain::set( const char * name, bool value, float min, float max ){
    return this->value.set( name, value, min, max );
}

