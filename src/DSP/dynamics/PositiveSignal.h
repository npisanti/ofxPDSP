
// HalfWavePeakDetector.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED
#define PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
    /*!
    @brief Make the signal unipolar by setting to zero negative values. Only positive values are not altered.
    
    This Unit set to zero all the negative input values. It is useful as first step for dynamic processing.

    */        
    
class PositiveSignal : public Formula{
        
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
        
};
    
typedef PositiveSignal HalfWavePeakDetector;
        
}


#endif  // PDSP_DYN_HALFWAVEPEAKDETECTOR_H_INCLUDED
