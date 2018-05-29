
// SoftClip.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SIGNAL_SOFTCLIP_H_INCLUDED
#define PDSP_SIGNAL_SOFTCLIP_H_INCLUDED


#include "../pdspCore.h"


namespace pdsp{
    
    /*!
    @brief Soft Clipper with settable threshold, useful for limiting signals.
    */

class SoftClip :  public Unit {

public:

    SoftClip();
    
    /*!
    @brief sets the default threshold and returns the unit ready to be patched.
    @param[in] freq new sample rate frequency
    
    */
    Patchable& set(float threshold );
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to decimate.
    */  
    Patchable& in_signal();
    
    
    /*!
    @brief Sets "threshdold" as selected input and returns this Unit ready to be patched. This is the used threshold in dB, not modulable in audio rate.
    */  
    Patchable& in_threshold();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output.
    */     
    Patchable& out_signal();     
            
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override;        
    void process (int bufferSize) noexcept override ;

    float constant;
    float t;

    OutputNode  output;
    InputNode   input_threshold;
    InputNode   input_signal;

};
    
    
    
}


#endif  // PDSP_SIGNAL_SOFTCLIP_H_INCLUDED
