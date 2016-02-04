
// PhazorShifter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_PHAZOR_PHAZORSHIFTERUNIT_H_INCLUDED
#define PDSP_PHAZOR_PHAZORSHIFTERUNIT_H_INCLUDED

#include "../../pdspCore.h"

namespace pdsp {
    /*!
    @brief Shift a phazor output, wrapping it around if needed.
    
    This units takes the out_phase() from a phazor and shift it by a given amount. It also wraps the phase values when they become >= 1.0f 
    
    */
    
class PhazorShifter : public Unit {
public:

    PhazorShifter();
    
    /*!
    @brief sets the default phase shift value and returns the unit ready to be patched.
    @param[in] phaseOffset phase shift value
    */
    Patchable& set(float phaseOffset);
    
    /*!
    @brief Sets "phase" as selected input and returns this Unit ready to be patched. This is the default input. This is the input for the phase buffer to shift.
    */ 
    Patchable& in_phase();
    
    /*!
    @brief Sets "shift" as selected input and returns this Unit ready to be patched. This input is the phase shift, it can be modulate at audio-rate to perform phase modulations.
    */ 
    Patchable& in_shift();
    
    /*!
    @brief Sets "phase" as selected output and returns this Unit ready to be patched. This is the default output. This is the shifted phase output to be patched into an oscillator in_phase(). 
    */ 
    Patchable& out_phase();
    
private:
    void prepareUnit ( int expectedBufferSize, double sampleRate ) override ;
    void releaseResources () override;
    void process(int bufferSize) noexcept override ;
    
    OutputNode output;
    InputNode input_phase;
    InputNode input_shift;

};


}//END NAMESPACE


#endif  // PDSP_PHAZOR_PHAZORSHIFTERUNIT_H_INCLUDED
