
// Saturator2.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SIGNAL_TANHSATURATOR_H_INCLUDED
#define PDSP_SIGNAL_TANHSATURATOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
        
/** 
@brief Saturator unit, tanh curve.

This unit saturates the signal using an hyperbolic tangent function. Generally this saturator is cleaner than Saturator1. If you want a better quality it could be useful to upsample the signal and set this unit to an higher oversampling level.
*/

class Saturator2 : public Formula {
        
private:
        
        float formula(const float &x) noexcept override;
        
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
     
};
        
}//END NAMESPACE


#endif  // PDSP_SIGNAL_TANHSATURATOR_H_INCLUDED
