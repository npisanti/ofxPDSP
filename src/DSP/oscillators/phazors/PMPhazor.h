
// PMPhazor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_PHAZOR_PMPHAZOR_H_INCLUDED
#define PDSP_PHAZOR_PMPHAZOR_H_INCLUDED


#include "../../pdspCore.h"

//add jitter output


namespace pdsp{
    /*!
    @brief Fully featured phazor with phase modulation and sync inputs.
    
    This is an oscillator phazor with phase modulation (aka FM) inputs, oscillator sync in/outs and out_inc() for antialiased oscillators.
    */
class PMPhazor :  public Unit {

public:
    PMPhazor();
    
    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the default input. This is the frequency of the phazor in hertz.
    */  
    Patchable& in_freq();
    
    /*!
    @brief Sets "pm" as selected input and returns this Unit ready to be patched. This is the phase modulation input. What we know as FM is usually phase modulation instead, so this is the "FM" input.
    */ 
    Patchable& in_pm();
    
    /*!
    @brief Sets "sync" as selected input and returns this Unit ready to be patched. This is the sync input. You have to patch an out_sync() to this. This input is not compatible with out_trig().
    */
    Patchable& in_sync();
    
    /*!
    @brief Sets "phase" as selected output and returns this Unit ready to be patched. This is the default output. This is the phase output to be patched into an oscillator in_phase(). 
    */ 
    Patchable& out_phase();

    /*!
    @brief Sets "inc" as selected output and returns this Unit ready to be patched. This is an output with values needed to perform some antialiasing algorithms, you patch it to the corrispondent in_inc() input.
    */ 
    Patchable& out_inc();
    
    /*!
    @brief Sets "sync" as selected output and returns this Unit ready to be patyhed. This is an output with values needed to perform oscillator sync, you patch it to the corrispondent in_sinc() input.
    */ 
    Patchable& out_sync();        
            
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override;        
    void process (int bufferSize) noexcept override ;

    float incCalculationMultiplier;

    template<bool pitchAR, bool syncAR, bool phaseAR>
    void process_audio(const float* freqBuffer, const float* trigBuffer, const float* pmBuffer, int bufferSize)noexcept;
    
    OutputNode  output_phase;
    OutputNode  output_inc;
    OutputNode  output_sync;
    InputNode   input_freq;
    InputNode   input_sync;
    InputNode   input_phase_mod;
    
    float   phase;
    float   phaseOffset;
    float   inc;
};



}


#endif  // PDSP_PHAZOR_PMPHAZOR_H_INCLUDED
