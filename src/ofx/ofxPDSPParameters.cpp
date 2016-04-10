
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

float ofxPDSPParameter::get() const{
    return node.getCRValue();
}


