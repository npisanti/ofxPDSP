
// Decimator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SIGNAL_DECIMATOR_H_INCLUDED
#define PDSP_SIGNAL_DECIMATOR_H_INCLUDED


#include "../pdspCore.h"


namespace pdsp{
    
    /*!
    @brief Sample Rate Decimator, reduce the sample rate of the input signal. New sample rate frequency is a real-time modulable input.
    */

class Decimator :  public Unit {

public:

    Decimator();
    
    /*!
    @brief sets the default frequency and returns the unit ready to be patched.
    @param[in] freq new sample rate frequency
    
    */
    Patchable& set(float freq);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to decimate.
    */  
    Patchable& in_signal();
    
    
    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the new sample rate.
    */  
    Patchable& in_freq();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output.
    */     
    Patchable& out_signal();     
            
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override;        
    void process (int bufferSize) noexcept override ;

    template<bool pitchAR>
    void process_audio(const float* inputBuffer, const float* freqBuffer, int bufferSize)noexcept;
    
    OutputNode  output;
    InputNode   input_freq;
    InputNode   input_signal;

    float   incCalculationMultiplier;        
    float   phase;
    float   inc;
    float   sampled;
};
    
    
    
}


#endif  // PDSP_SIGNAL_DECIMATOR_H_INCLUDED
