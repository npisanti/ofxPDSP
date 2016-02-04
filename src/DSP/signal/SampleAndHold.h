
// SampleAndHold.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SIGNAL_SAMPLEHOLD_H_INCLUDED
#define PDSP_SIGNAL_SAMPLEHOLD_H_INCLUDED

#include "../pdspCore.h"


namespace pdsp{
    
/** 
@brief Sample & Hold unit.

This unit continously outputs the last sampled signal. Each time a trigger on value is found in in_trig() the input in in_signal() is sampled.
*/

class SampleAndHold : public Unit{

public:
    SampleAndHold();

    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. The signal of this input will be sampled on trigger.
    */     
    Patchable& in_signal();
  
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. On a trigger on value the signal is sampled,
    */ 
    Patchable& in_trig();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. Outputs the last sampled value.
    */ 
    Patchable& out_signal();

    /*!
    @brief returns the last sampled value. This method is thread-safe.
    */
    float meter_out() const;

private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override;
    void process (int bufferSize) noexcept override ;

    template<bool smoothed>
    void process_audio (int bufferSize) noexcept ;

    OutputNode output;

    InputNode input_trig;
    InputNode input_signal;
    
    float sampled;
    
    std::atomic<float> meter;
};

} // pdsp namespace end


#endif  // PDSP_SIGNAL_SAMPLEHOLD_H_INCLUDED
