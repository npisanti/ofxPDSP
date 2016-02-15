
// FreqToMs.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FREQTOMS_H_INCLUDED
#define PDSP_FREQTOMS_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
/** 
@brief output is frequency converted to milliseconds, useful for tuning delays

*/    

class FreqToMs : public Formula {
public:

    /** 
    @brief static function to convert freq to ms without constructing an object
    @param[in] value pitch to convert to ms
    */    
    static float eval(float value);
    
protected:
    
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
 
};
        
}//END NAMESPACE






#endif  // PDSP_FREQTOMS_H_INCLUDED
