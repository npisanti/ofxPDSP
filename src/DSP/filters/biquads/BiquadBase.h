
// BiquadBase.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FILTERS_BIQUADBASE_H_INCLUDED
#define PDSP_FILTERS_BIQUADBASE_H_INCLUDED

#include "../../pdspCore.h"

namespace pdsp{
    
    /*!
    @brief Base class for biquad filters
    
    Base class for implementation of biquadratic filters, biquad filters implemented in direct form I normalized to a0. This filter operates only on audio-rate signals.
    */     

class BiquadBase : public Unit{
        
        
public:
    BiquadBase();

    /*!
    @brief sets the default filter values and returns the unit ready to be patched.
    @param[in] freq frequency at which the filter operates
    @param[in] Q Q (quality factor) of some filters
    @param[in] gain gain in dB in case of some eq filters, if not specified 0.0f is passed as argument
    */
    Patchable& set(float freq, float Q, float gain=0.0f);

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    */   
    Patchable& in_signal();

    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the frequency at which the filter operates, broadly speaking.
    */       
    Patchable& in_freq();
    
    /*!
    @brief Sets "Q" as selected input and returns this Unit ready to be patched. This is the quality factor of the filter, usually it means how the filter peak or notch is wide at the cutoff frequency.
    */       
    Patchable& in_Q();

    /*!
    @brief Sets "gain" as selected input and returns this Unit ready to be patched. This is the gain in dB of some eq filter like peak or shelving EQ filters.
    */    
    Patchable& in_gain();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the filter output.
    */ 
    Patchable& out_signal();

protected:

    /*!
    @brief the function that cooks the biquad variables, remember to divide eveything for a0
    @param[in] sampleRate
    @param[in] frequency
    @param[in] Q
    @param[in] dBgain
    */
    virtual void cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain);

    float b0;
    float b1;
    float b2;

    float a1;
    float a2;
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;        

    OutputNode output;
    InputNode input_signal;
    
    InputNode input_frequency;
    InputNode input_Q;
    InputNode input_gain;

    float x_z1;
    float x_z2;
    float y_z1;
    float y_z2;
    float sampleRate;
};
            
} // end pdsp namespace 

#endif  // PDSP_FILTERS_BIQUADBASE_H_INCLUDED
