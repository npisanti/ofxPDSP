
// LFOPhasor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_PHAZOR_LFOPHAZOR_H_INCLUDED
#define PDSP_PHAZOR_LFOPHAZOR_H_INCLUDED


#include "../../pdspCore.h"


namespace pdsp{
    
    /*!
    @brief Simple phazor for LFOs.
    
    This is a simple phazor, with inputs for controlling the frequency, the phazor retrigger, the phazor start, and with phase output and a "trig" output useful for triggering other units.
    */

class LFOPhasor :  public Unit {

public:

    LFOPhasor();
    
    /*!
    @brief sets the default frequency and phase start values and returns the unit ready to be patched.
    @param[in] freq frequency to set
    @param[in] phase_start starting phase to set
    */
    Patchable& set(float freq, float phase_start=0.0f);
    
    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the default input. This is the frequency of the phazor in hertz.
    */  
    Patchable& in_freq();
    
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. Connect a "trig" output to this input to activate retriggering of the phazor on trigger on values.
    */  
    Patchable& in_retrig();
  
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. Connect a "trig" output to this input to activate retriggering of the phazor on trigger on values.
    */  
    Patchable& in_trig();
    
    /*!
    @brief Sets "phase_start" as selected input and returns this Unit ready to be patched. This is the starting value of the phazor on retrigger. It is updated only at control-rate.
    */
    Patchable& in_phase_start();
    
    /*!
    @brief Sets "phase" as selected output and returns this Unit ready to be patched. This is the default output. This is the phase output to be patched into an oscillator in_phase(). 
    */ 
    Patchable& out_phase();
    
    /*!
    @brief Sets "trig" as selected output and returns this Unit ready to be patched. This outputs sets a trigger on value each time the phazor restarts its cycle, it can be useful to trigger other units.
    */     
    Patchable& out_trig();     
            
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override;        
    void process (int bufferSize) noexcept override ;

    template<bool pitchAR, bool trigAR>
    void process_audio(const float* freqBuffer, const float* trigBuffer, int bufferSize)noexcept;
    
    void onReset(const int &n, const float &inc, float* &triggerBuffer, const int &bufferSize);
    
    OutputNode  output_phase;
    OutputNode  output_trig;
    InputNode   input_freq;
    InputNode   input_retrig;
    InputNode   input_phase_start;

    float incCalculationMultiplier;        
    float   phase;
    float   inc;
};
    
    
    
}


#endif  // PDSP_PHAZOR_LFOPHAZOR_H_INCLUDED
