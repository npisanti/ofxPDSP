
#include "Parameter.h"

pdsp::Parameter::Parameter(){
    addModuleOutput("signal", valueControl);
    
    parameter.addListener(this, &pdsp::Parameter::onSet);
    parameter_i.addListener(this, &pdsp::Parameter::onSetI);
    parameter_b.addListener(this, &pdsp::Parameter::onSetB);
    parameter.set( 0.0f );
    parameter_i.set( 0 );
    
    boolmin = 0.0f;
    boolmax = 1.0f;
    
    mode = 0;
    bCanUpdate = true;
}

pdsp::Parameter::Parameter(const pdsp::Parameter & other) : pdsp::Parameter(){
    this->parameter = other.parameter;
    this->parameter_i = other.parameter_i;
    this->parameter_b = other.parameter_b;
    this->valueControl.set ( other.get() );
    this->mode = other.mode;
    bCanUpdate = true;
}

pdsp::Parameter& pdsp::Parameter::operator=(const pdsp::Parameter & other){
    this->parameter = other.parameter;
    this->parameter_i = other.parameter_i;
    this->parameter_b = other.parameter_b;
    this->valueControl.set ( other.get() );
    this->mode = other.mode;
    bCanUpdate = true;
    return *this;
}

pdsp::Parameter::~Parameter(){
    parameter.removeListener(this, &pdsp::Parameter::onSet);
    parameter_i.removeListener(this, &pdsp::Parameter::onSetI);
    parameter_b.removeListener(this, &pdsp::Parameter::onSetB);
}

ofParameter<float>& pdsp::Parameter::set(const char * name, float value, float min, float max){
    if( mode==modeInt ){
        mode = modeCombined;
    }else{
        mode = modeFloat;
    }
    parameter.set(name, value, min, max);
    return parameter;
}

ofParameter<float>& pdsp::Parameter::set(const char * name, float min, float max){
    parameter.set(name, parameter.get(), min, max);
    if( mode==modeInt ){
        mode = modeCombined;
    }else{
        mode = modeFloat;
    }
    return parameter;
}

ofParameter<int>& pdsp::Parameter::set(const char * name, int value, int min, int max){
    if( mode==modeFloat ){
        mode = modeCombined;
    }else{
        mode = modeInt;
    }
    parameter_i.set(name, value, min, max);
    return parameter_i;
}

ofParameter<int>& pdsp::Parameter::set(const char * name, int min, int max){
    parameter.set(name, parameter_i.get(), min, max);
    if( mode==modeFloat ){
        mode = modeCombined;
    }else{
        mode = modeInt;
    }
    return parameter_i;
}

ofParameter<bool>& pdsp::Parameter::set(const char * name, bool value, float min, float max ){
    boolmin = min;
    boolmax = max;
    mode = modeBool;
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

void pdsp::Parameter::enableSmoothing(float timeMs){
    valueControl.enableSmoothing( timeMs );
    
}

void pdsp::Parameter::disableSmoothing(){
    valueControl.disableSmoothing();
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

ofParameter<bool>& pdsp::Parameter::set( std::string name, bool value, float min, float max  ) {
    return set( name.c_str(), value, min, max );
}

ofParameter<float>& pdsp::Parameter::set(const char * name, double value, double min, double max){
    return set( name, float(value), float(min), float(max) );
}

ofParameter<float>& pdsp::Parameter::set(const char * name, double min, double max){
    return set( name, float(min), float(max) );
}

ofParameter<float>& pdsp::Parameter::set( std::string name, double value, double min, double max){
    return set( name, float(value), float(min), float(max) );
}

ofParameter<float>& pdsp::Parameter::set( std::string name, double min, double max){
    return set( name, float(min), float(max) );
}

void pdsp::Parameter::onSet(float &newValue){
    if( bCanUpdate ){
        valueControl.set( parameter.get() + static_cast<float>(parameter_i.get()) );
    }
}

void pdsp::Parameter::onSetI(int  &newValue){
    if( bCanUpdate ){
        valueControl.set( parameter.get() + static_cast<float>(parameter_i.get()) );
    }
}

void pdsp::Parameter::onSetB(bool  &newValue){
    if(bCanUpdate){
        if( newValue ){
            valueControl.set( boolmax );
        }else{
            valueControl.set( boolmin );
        }        
    }
}

void pdsp::Parameter::set( float value ){
    bCanUpdate = false;
    
    switch( mode ){
        case modeInt: 
            valueControl.set( value );
            parameter_i.set( value ); 
        break;
        
        case modeFloat: 
            valueControl.set( value );
            parameter.set( value ); 
        break;
        
        case modeCombined:
        {
            int integerpart = value;
            float fractpart = value - float(integerpart);
            parameter.set( fractpart );
            parameter_i.set( integerpart ); 
            valueControl.set( value );
        }
        break;
        
        case modeBool:
        {
            float average = (boolmin+boolmax)*0.5f;
            if( value > average ){
                parameter_b.set( true );
                valueControl.set( boolmax );
            }else{
                parameter_b.set( false );
                valueControl.set( boolmin );
            }
        }
        break;
        
        default : 
            valueControl.set( value ); 
        break;
    }
    
    bCanUpdate = true;
}
