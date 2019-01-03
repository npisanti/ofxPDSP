
// OnTrigger.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2019


#ifndef PDSP_UTILITY_ONTRIGGER_H_INCLUDED
#define PDSP_UTILITY_ONTRIGGER_H_INCLUDED

#include "../pdspCore.h"


namespace pdsp{
    
/** 
@brief OnTrigger 

Calls an user assignable onTrigger( float value ) function when a trigger is received. 
*/

class OnTrigger : public Unit{

public:
    OnTrigger();

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. The signal of this input will be sampled on trigger.
    */     
    Patchable& in_signal();
  
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. This is the default input.  On a trigger signal the onTrigger function is called. Triggers with value 0.0f will not trigger the function.
    */ 
    Patchable& in_trig();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. Outputs the last generated value.
    */ 
    Patchable& out_signal();

    /*!
    @brief returns the last generated value. This method is thread-safe.
    */
    float meter_output() const;
    
    /*!
    @brief function to assign, by default it outputs the input. 
    */
    std::function<float(float)> code;

private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override;
    void process (int bufferSize) noexcept override ;

    OutputNode output;

    InputNode input_trig;
    
    float sampled;
    
    std::atomic<float> meter;
};

} // pdsp namespace end


#endif  // PDSP_UTILITY_ONTRIGGER_H_INCLUDED
