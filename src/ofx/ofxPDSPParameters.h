
// ofxPDSPParameters.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PARAMETERS_H_INCLUDED
#define OFXPDSP_PARAMETERS_H_INCLUDED

#include "../DSP/pdspCore.h"

#include "ofMain.h"

//-------------------------------------------------------------------------------------------------

class ofxPDSPParameter{
    
public:
    ofxPDSPParameter();
    ~ofxPDSPParameter();
    
    pdsp::ValueNode& set(const char * name, float value, float min, float max);
    pdsp::ValueNode& set(float value);
    
    float get();
    
    ofParameter<float>  parameter;
    pdsp::ValueNode     node;

private:
    void onSet(float &parameter);
    
};

//-------------------------------------------------------------------------------------------------

class ofxPDSPToggle{
    
public:
    ofxPDSPToggle();
    ~ofxPDSPToggle();
    
    pdsp::ValueNode& set(const char * name, bool active, float low, float high);
    pdsp::ValueNode& set(bool high);
    
    ofParameter<bool>  parameter;
    pdsp::ValueNode     node;
    
private:
    void onSet(bool &parameter);
    float low;
    float high;
};





#endif //OFXPDSP_PARAMETERS_H_INCLUDED
