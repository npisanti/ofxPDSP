
// LinToDB.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTIL_LINTODB_H_INCLUDED
#define PDSP_UTIL_LINTODB_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
/** 
@brief Converts linear values into dB values

*/    

class LinToDB : public Formula {
       
public:

    /** 
    @brief static function that performs linear to dB operation on a given value and returns the results
    @param[in] value value to convert
    */   
    static float eval(float value);
    
protected:
    
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;

};
        
}//END NAMESPACE


#endif  // LINTODB_H_INCLUDED
