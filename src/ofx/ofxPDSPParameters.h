
// ofxPDSPParameters.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PARAMETERS_H_INCLUDED
#define OFXPDSP_PARAMETERS_H_INCLUDED

#include "../DSP/pdspCore.h"

#include "ofMain.h"

//-------------------------------------------------------------------------------------------------

/*!
@brief Utility class to bind a pdsp::ValueNode to a ofParameter.

ofxPDSPParameter conteins both an ofParameter<float> and an pdsp::ValueNode. When you tweak the ofParameter the ValueNode is also updated, and viceversa. You can patch the pdsp::ValueNode to your modules.

*/
class ofxPDSPParameter{
    
public:
    ofxPDSPParameter();
    ~ofxPDSPParameter();
    
    /*!
    @brief sets the parameters
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min minimum value 
    @param[in] max maximum value
    */    
    pdsp::ValueNode& set(const char * name, float value, float min, float max);

    /*!
    @brief sets the value 
    @param[in] value new value
    */   
    pdsp::ValueNode& set(float value);

    /*!
    @brief gets the value
    */       
    float get() const;
    
    /*!
    @brief the ofParameter
    */       
    ofParameter<float>  parameter;
    
    /*!
    @brief the pdsp::Value to patch
    */       
    pdsp::ValueNode     node;

private:
    void onSet(float &parameter);
    
};






#endif //OFXPDSP_PARAMETERS_H_INCLUDED
