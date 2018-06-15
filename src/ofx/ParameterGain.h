
// ParameterGain.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef OFXPDSP_PDSPSTEREOFADER_H_INCLUDED
#define OFXPDSP_PDSPSTEREOFADER_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "../DSP/utility/DBtoLin.h"
#include "Parameter.h"

#include "ofMain.h"

//-------------------------------------------------------------------------------------------------

/*!
@brief Two pdsp::Amp with the mod controlled in decibel by a pdsp::Parameter converted from, packed together. Multichannel.

This will save you lot of time to mix signals. A value lower than -48.0f will set the amp mod to 0.0f, muting the input signal and pausing its DSP processing.
*/

namespace pdsp{
    
class ParameterGain : public pdsp::Patchable {
    
public:
    ParameterGain();
    ParameterGain(const ParameterGain & other);
    ParameterGain& operator=(const ParameterGain & other);
    ~ParameterGain();
        
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
    @brief sets the and returns the internal ofParameter<bool>, useful to set up an UI. This parameter set the value to min when false and to max when true, 0.0f and 1.0f if not given.
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min value when false, 0,0f if not given
    @param[in] max value when true, 1.0f if not given
    */    
    ofParameter<bool>& set( std::string name, bool value, float min=0.0f, float max=1.0f );
    
    /*!
    @brief sets the and returns the internal ofParameter<bool>, useful to set up an UI. This parameter set the value to min when false and to max when true, 0.0f and 1.0f if not given.
    @param[in] name this will become the name of the ofParameter
    @param[in] value default value
    @param[in] min value when false, 0,0f if not given
    @param[in] max value when true, 1.0f if not given
    */    
    ofParameter<bool>& set( const char * name, bool value, float min=0.0f, float max=1.0f  );
    
    
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
    @brief sets the number of different inputs this module can process. Be sure to set the channels before doing any patching. You can access channels with in/out methods, using a string with their number as input, for example in("1") or out("4"), or by using ini(int ch) or outi(int ch) methods.
    @param[in] size number of channels
    */        
    void channels( int size );    
    
    /*!
    @brief Returns the input of the given channels.
    @param[in] ch channel index
    */      
    Patchable& ini( int ch );

    /*!
    @brief Returns the output of the given channel.
    @param[in] ch channel index
    */  
    Patchable& outi( int ch );


    /*!
    @brief returns the actual output value. Thread-safe.
    @param[in] ch channel index
    */ 
    float meter_output( int ch=0 ) const;     

    /*!
    @brief returns the actual output of the channel 0. Thread-safe.
    */ 
    float meter_0() const;     

    /*!
    @brief returns the actual output of the channel 1. Thread-safe.
    */     
    float meter_1() const;     

    /*!
    @brief returns the actual output of the channel 0. Thread-safe.
    */ 
    float meter_L() const;     

    /*!
    @brief returns the actual output of the channel 1. Thread-safe.
    */     
    float meter_R() const;     

    /*!
    @brief returns the actual control value. Thread-safe.
    */ 
    float meter_mod() const;     
    
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */     
    Patchable& in_1();
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2.
    */      
    Patchable& in_L();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */     
    Patchable& in_R();
    

    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_1();
            
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_R();

private:

    void patch();
    
    pdsp::Parameter value;
    
    std::vector<Amp*> amps;
    pdsp::DBtoLin   dBtoLin;

};

}

#endif // OFXPDSP_PDSPSTEREOFADER_H_INCLUDED
