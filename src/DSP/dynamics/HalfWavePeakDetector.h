
// HalfWavePeakDetector.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED
#define PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    /*!
    @brief Make the signal unipolar by setting to zero negative values.
    
    This Unit set to zero all the negative input values. It is useful as first step for dynamic processing.

    */        
    
class HalfWavePeakDetector : public Formula{
        
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
        
};
        
        
}


#endif  // PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED
