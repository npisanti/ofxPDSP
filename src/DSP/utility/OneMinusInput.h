
// OneMinusInput.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_ONEMINUSINPUT_H_INCLUDED
#define PDSP_ONEMINUSINPUT_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
/** 
@brief output is 1.0f - input

*/    

class OneMinusInput : public Formula {
        
protected:
    
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
 
};
        
}//END NAMESPACE






#endif  // PDSP_ONEMINUSINPUT_H_INCLUDED
