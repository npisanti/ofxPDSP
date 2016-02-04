
// FullWavePeakDetector.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED
#define PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
        
    /*!
    @brief Make the signal unipolar by mirroring values that are less than zero.
    
    This Unit mirrors all the values tha are less than zero. It is useful as first step for dynamic processing.

    */
        
class FullWavePeakDetector : public Formula{
        
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
        
};
        
        
}



#endif  // PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED
