
// DBtoLin.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTIL_DBTOLIN_H_INCLUDED
#define PDSP_UTIL_DBTOLIN_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
/** 
@brief Converts dB values into linear values

*/    

class DBtoLin : public Formula {
public:

    /** 
    @brief static function that performs dB to linear operation on a given value and returns the results
    @param[in] value value to convert
    */   
    static float eval(float value);
    
    /** 
    @brief sets the minimum db value below which 0.0 is given as result
    @param[in] silenceDB value to convert into 
    */ 
    static void setSilenceThreshold( float silenceDB );
    
protected:
    
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;

private:
    static float silenceThreshold;
};
        
}//END NAMESPACE


#endif  // DBTOLIN_H_INCLUDED
