
// BipolarToUnipolar.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_UTILITY_BIPOLARTOUNIPOLAR_H_INCLUDED
#define PDSP_UTILITY_BIPOLARTOUNIPOLAR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
/** 
@brief Converts bipolar signals to unipolar.

This unit converts a signal between -1.0f and 1.0f to a signal between 0.0f and 1.0f
*/
class BipolarToUnipolar : public Formula {
        
private:
    
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
     
};
        
}//END NAMESPACE






#endif  // PDSP_UTILITY_BIPOLARTOUNIPOLAR_H_INCLUDED
