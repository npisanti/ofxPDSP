
// AbsoluteSignal.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED
#define PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
        
    /*!
    @brief The output are the absolute values of the input signal values. Make the signal unipolar by mirroring values that are less than zero. 
    
    This Unit mirrors all the values tha are less than zero. It is useful as first step for dynamic processing.

    */
        
class AbsoluteSignal : public Formula{
        
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
        
};
        
typedef AbsoluteSignal FullWavePeakDetector;        

}



#endif  // PDSP_DYN_FULLWAVEPEAKDETECTOR_H_INCLUDED
