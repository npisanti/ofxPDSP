
#include "ofxPDSPParameters.h"

ofxPDSPParameter::ofxPDSPParameter(){
    parameter.addListener(this, &ofxPDSPParameter::onSet);
}

ofxPDSPParameter::~ofxPDSPParameter(){
    parameter.removeListener(this, &ofxPDSPParameter::onSet);
}

pdsp::ValueNode& ofxPDSPParameter::set(const char * name, float value, float min, float max){
    parameter.set(name, value, min, max);
    return node;
}

pdsp::ValueNode& ofxPDSPParameter::set(float value){
    parameter.set(value);
    return node;
}

void ofxPDSPParameter::onSet(float &parameter){
    node.set(parameter);
}

float ofxPDSPParameter::get(){
    return node.getCRValue();
}

//-------------------------------------------------------------------------------------------------

ofxPDSPToggle::ofxPDSPToggle(){
    parameter.addListener(this, &ofxPDSPToggle::onSet);
}

pdsp::ValueNode& ofxPDSPToggle::set(const char * name, bool active, float low, float high){
    parameter.set(name, active);
    this->low  = low;
    this->high = high;
    return node;
}

pdsp::ValueNode& ofxPDSPToggle::set(bool active){
    parameter.set(active);
    return node;
}

void ofxPDSPToggle::onSet(bool &parameter){
    if( parameter ){
        node.set(high);
    }else{
        node.set(low);
    }

}
