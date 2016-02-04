
// SquarePeakDetector.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DYN_MSDETECTOR_H_INCLUDED
#define PDSP_DYN_MSDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    
    /*!
    @brief Make the signal unipolar by multiplying the values for themselves.
    
    This Unit make the square of the input values. It is useful as first step for dynamic processing.
    */        
    
class SquarePeakDetector : public Formula{
        
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
        
};
        
} // end pdsp namespace


#endif  // PDSP_DYN_MSDETECTOR_H_INCLUDED
