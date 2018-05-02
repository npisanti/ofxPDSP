
// ofxPDSPValue.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PDSPVALUE_H_INCLUDED
#define OFXPDSP_PDSPVALUE_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "../DSP/helpers/UsesSlew.h"
#include "../DSP/control/TriggerControl.h"

#include "ofMain.h"

typedef pdsp::TriggerControl ofxPDSPTrigger;

//-------------------------------------------------------------------------------------------------

/*!
@brief Utility class control the audio dsp parameter and bind them to one or more internal ofParameter

ofxPDSPValue contains some ofParameter and an internal atomic float value, that is read and processed in a thread-safe manner. When one of the ofParameters is changed also the internal value is changed. The setv() methods sets only the internal value, for setting parameter faster when you don't need the ofParameter features. The output of this class can be patched to the audio DSP and can be optionally smoothed out.

*/
class ofxPDSPValue : public pdsp::Unit, public pdsp::UsesSlew {
    
public:
    ofxPDSPValue();
    ofxPDSPValue(const ofxPDSPValue & other);
    ofxPDSPValue& operator=(const ofxPDSPValue & other);
    
    ~ofxPDSPValue();
    
    /*!
    @brief sets the and returns the internal ofParameter<float>, useful to set up an UI. This parameter is summed to another ofParameter<int>, that you access passing all ints as parameters
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min minimum value 
    @param[in] max maximum value
    */    
    ofParameter<float>& set(const char * name, float value, float min, float max);

    /*!
    @brief sets the and returns the internal ofParameter<int>, useful to set up an UI. This parameter is summed to another ofParameter<float>, that you access passing all ints as parameters
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min minimum value 
    @param[in] max maximum value
    */    
    ofParameter<int>& set(const char * name, int value, int min, int max);


    /*!
    @brief sets the value min and max boundary when operated by the ofParameter<float> in the UI and returns the parameter ready to be added to the UI
    @param[in] name this will become the name of the ofParameter
    @param[in] min minimum value 
    @param[in] max maximum value
    */  
    ofParameter<float>& set(const char * name, float min, float max);

    /*!
    @brief sets the value min and max boundary when operated by the ofParameter<int> in the UI and returns the parameter ready to be added to the UI
    @param[in] name this will become the name of the ofParameter
    @param[in] min minimum value 
    @param[in] max maximum value
    */  
    ofParameter<int>& set(const char * name, int min, int max);

    /*!
    @brief sets the and returns the internal ofParameter<float>, useful to set up an UI. This parameter is summed to another ofParameter<int>, that you access passing all ints as parameters
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min minimum value 
    @param[in] max maximum value
    */    
    ofParameter<float>& set( std::string name, float value, float min, float max);

    /*!
    @brief sets the and returns the internal ofParameter<int>, useful to set up an UI. This parameter is summed to another ofParameter<float>, that you access passing all ints as parameters
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min minimum value 
    @param[in] max maximum value
    */    
    ofParameter<int>& set( std::string name, int value, int min, int max);


    /*!
    @brief sets the value min and max boundary when operated by the ofParameter<float> in the UI and returns the parameter ready to be added to the UI
    @param[in] name this will become the name of the ofParameter
    @param[in] min minimum value 
    @param[in] max maximum value
    */  
    ofParameter<float>& set( std::string name, float min, float max);

    /*!
    @brief sets the value min and max boundary when operated by the ofParameter<int> in the UI and returns the parameter ready to be added to the UI
    @param[in] name this will become the name of the ofParameter
    @param[in] min minimum value 
    @param[in] max maximum value
    */  
    ofParameter<int>& set( std::string name, int min, int max);

    /*!
    @brief sets the and returns the internal ofParameter<bool>, useful to set up an UI. This parameter set the value to 0.0f when false and to 1.0f when true.
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    */    
    ofParameter<bool>& set( std::string name, bool value );
    
    /*!
    @brief sets the and returns the internal ofParameter<bool>, useful to set up an UI. This parameter set the value to 0.0f when false and to 1.0f when true.
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    */    
    ofParameter<bool>& set( const char * name, bool value );


    /*!
    @brief sets the value without updating the ofParameters
    @param[in] value new value
    
    This set method don't update the ofParameter, for faster computation when you're not using this class for UI
    */   
    void setv(float value);


    /*!
    @brief returns the ofParameter ready to be added to the UI
    */  
    ofParameter<float>& getOFParameterFloat();

    /*!
    @brief returns the ofParameter ready to be added to the UI
    */  
    ofParameter<int>& getOFParameterInt();

    /*!
    @brief returns the ofParameter ready to be added to the UI
    */  
    ofParameter<bool>& getOFParameterBool();


    /*!
    @brief enables the smoothing of the setted values
    @param[in] timeMs how many milliseconds will take to reach the setted value
    */  
    void enableSmoothing(float timeMs);
    
    /*!
    @brief disable the smoothing of the setted values. smoothing is disabled by default
    */  
    void disableSmoothing();

    /*!
    @brief gets the value
    */       
    float get() const;

    /*!
    @brief returns the actual output value. Thread-safe.
    */ 
    float meter_output() const;     

private:

    ofParameter<float>  parameter;
    ofParameter<int>    parameter_i;
    ofParameter<bool>   parameter_b;
    
    pdsp::OutputNode output;
    
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    atomic<float> lastValue;
    atomic<float> value;
    
    void onSet(float &newValue);
    void onSetI(int   &newValue);
    void onSetB(bool   &newValue);

    
};






#endif //OFXPDSP_PDSPVALUE_H_INCLUDED
