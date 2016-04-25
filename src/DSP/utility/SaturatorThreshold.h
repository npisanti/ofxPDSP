
// SaturatorThreshold.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTILITY_SATURATORTHRESHOLD_H_INCLUDED
#define PDSP_UTILITY_SATURATORTHRESHOLD_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

    /*!
    @brief An unit for a send/return for a Saturator formula. The saturator operates only on the signal above a settable threshold.
    
    The threshold is used bipolarly on both for positive and negative signals, for asymmetrical clipping use BipolarToUnipolar first.
    */

class SaturatorThreshold : public Unit{

public:

    SaturatorThreshold();
    
    /*!
    @brief 
    @param[in] threshold
    */
    Patchable& set(float threshold);
    
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal/value to slew.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "threshold" as selected input and returns this Unit ready to be patched. This input is the threshold in linear level of the saturation. Usually works well in the -6dB to -0.1dB.
    */   
    Patchable& in_threshold();

    /*!
    @brief Sets "return" as selected input and returns this Unit ready to be patched. This the return channel, patch the saturator output to this input;
    */   
    Patchable& in_return();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the output of the saturated signal.
    */   
    Patchable& out_signal();

    /*!
    @brief Sets "send" as selected input and returns this Unit ready to be patched. This the send output, patch it to the saturator input.
    */   
    Patchable& out_send();
    
    
private:

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    InputNode input;
    InputNode input_threshold;
    InputNode input_return;
    OutputNode output;
    OutputNode send;

    float threshold;

    
};
        
        
}//END NAMESPACE

#endif  // PDSP_UTILITY_SATURATORTHRESHOLD_H_INCLUDED
