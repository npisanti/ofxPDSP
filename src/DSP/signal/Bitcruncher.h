
// Bitcruncher.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SIGNAL_BITCRUNCHER_H_INCLUDED
#define PDSP_SIGNAL_BITCRUNCHER_H_INCLUDED

#include "../pdspCore.h"


namespace pdsp{
    
/** 
@brief Bitcruncher. Makes the output distorted by reducing the bit used for storing the amplitude values

*/

class Bitcruncher : public Unit{

public:
    Bitcruncher();

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. The signal of this input will be sampled on trigger.
    */     
    Patchable& in_signal();
  
    /*!
    @brief Sets "bits" as selected input and returns this Unit ready to be patched. Bits 
    */ 
    Patchable& in_bits();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. Outputs the last sampled value.
    */ 
    Patchable& out_signal();


private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override;
    void process (int bufferSize) noexcept override ;

    template<bool bitsAR>
    void process_audio(const float* &inputBuffer, const float* &bitsBuffer, const int &bufferSize)noexcept;

    float multiply;
    float scaleBack;
    
    OutputNode output;

    InputNode input_bits;
    InputNode input_signal;
    
};

} // pdsp namespace end


#endif  // PDSP_SIGNAL_BITCRUNCHER_H_INCLUDED
