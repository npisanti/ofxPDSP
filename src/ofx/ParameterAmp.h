
// ParameterAmp.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PDSPVALUEMULTIPLIER_H_INCLUDED
#define OFXPDSP_PDSPVALUEMULTIPLIER_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "Parameter.h"

#include "ofMain.h"

//-------------------------------------------------------------------------------------------------

/*!
@brief A pdsp::Amp with the mod controlled by a pdsp::Parameter, packed together. Multichannel.

This will save you lot of time when making monophonic synths. It has an input and output like a pdsp::Amp, but no in_mod() as the mod is controlled by an internal pdsp::Parameter you can access and set with the other methods.
*/

namespace pdsp{

class ParameterAmp : public pdsp::Patchable {
    
public:
    ParameterAmp();
    ParameterAmp(const ParameterAmp & other);
    ParameterAmp& operator=(const ParameterAmp & other);
    ~ParameterAmp();
        
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
    @brief returns the actual output value. Thread-safe.
    @param[in] ch channel index
    */ 
    float meter_output( int ch=0 ) const;     

    /*!
    @brief returns the actual control value. Thread-safe.
    */ 
    float meter_mod() const;     
    
    /*!
    @brief Sets "0" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal/value to multiply.
    */   
    pdsp::Patchable& in_signal();
    
    /*!
    @brief Sets "0" as selected output and returns this Unit ready to be patched. This is the default output. This is the product of in("signal") multiplied by controlling value.
    */ 
    pdsp::Patchable& out_signal();
    
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


private:

    void patch();
    
    pdsp::Parameter value;
    
    std::vector<Amp*> amps;
};

}

#endif //OFXPDSP_PDSPVALUEMULTIPLIER_H_INCLUDED
