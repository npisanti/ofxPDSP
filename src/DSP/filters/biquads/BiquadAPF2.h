
// BiquadAPF2.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DSP_BIQUADS_APF2_H_INCLUDED
#define PDSP_DSP_BIQUADS_APF2_H_INCLUDED

#include "BiquadBase.h"

namespace pdsp{
    /*!
    @brief 2pole All Pass Filter
    */
class BiquadAPF2 : public BiquadBase {

private:
    void cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain) override;

};


} // pdsp namespace end


#endif // PDSP_DSP_BIQUADS_APF2_H_INCLUDED
