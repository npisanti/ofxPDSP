
// ValueControl.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PDSPCONTROLVALUE_H_INCLUDED
#define OFXPDSP_PDSPCONTROLVALUE_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "../DSP/helpers/UsesSlew.h"

#include "ofMain.h"

//-------------------------------------------------------------------------------------------------

/*!
@brief Thread-safe value control with smoothing.

pdsp::ValueControl contains a value that you can patch to other pdsp modules, and set with the set() method. The value can optionally be smoothed over time, and the set() and get() methods are thread-safe.

*/

namespace pdsp{

class ValueControl : public pdsp::Unit, public pdsp::UsesSlew {
    
public:
    ValueControl();
    ValueControl(const ValueControl & other);
    ValueControl& operator=(const ValueControl & other);
    

    /*!
    @brief sets the value, thread safe.
    @param[in] value new value
    
    */   
    void set(float value){ this->value.store( value );}

    /*!
    @brief gets the value
    */       
    float get() const { return value.load(); }

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
    @brief returns the actual output value. Thread-safe.
    */ 
    float meter_output() const { return slewLastValue.load(); }     

private:

    pdsp::OutputNode output;
    
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    std::atomic<float> lastValue;
	std::atomic<float> value;
    
};

}

#endif //OFXPDSP_PDSPCONTROLVALUE_H_INCLUDED
