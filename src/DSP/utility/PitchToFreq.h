
// PitchToFreq.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTIL_PITCHTOFREQUENCY_H_INCLUDED
#define PDSP_UTIL_PITCHTOFREQUENCY_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

/** 
@brief Converts pitch values to frequency values 

Converts pitch values to frequency values  given a reference tuning for A3. Standard tuning is A3=440hz
*/    
          
class PitchToFreq : public Formula {
    
public:

    PitchToFreq();

    /** 
    @brief static function that sets the reference tuning frequency, standard is 440hz
    @param[in] tuningFrequency reference frequency
    */   
    static void setTuning(float tuningFrequency);
    
    /** 
    @brief static function to convert pitch to freq without constructing an object
    @param[in] value pitch to convert to freq
    */    
    static float eval(float value);
    
    /** 
    @brief static function that returns the reference tuning frequency
    */
    static float getTuningFrequency();
           
private:
        
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
    
    static float freqCalculationMultiplier;
    static float tuning;
    
};
            

    
}//END NAMESPACE




#endif  // PITCHTOFREQUENCY_H_INCLUDED
