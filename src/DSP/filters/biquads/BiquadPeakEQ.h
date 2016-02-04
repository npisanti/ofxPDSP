
// BiquadPeakEQ.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DSP_BIQUADS_PEAKEQ_H_INCLUDED
#define PDSP_DSP_BIQUADS_PEAKEQ_H_INCLUDED

#include "BiquadBase.h"

namespace pdsp{
    /*!
    @brief Peaking EQ filter with variable Q
    */
class BiquadPeakEQ : public BiquadBase {
    
private: 
    void cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain) override;
    
};
    
    
} // pdsp namespace end


#endif // PDSP_DSP_BIQUADS_PEAKEQ_H_INCLUDED
