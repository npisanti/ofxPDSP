
// Tunable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CORE_TUNABLE_H_INCLUDED
#define PDSP_CORE_TUNABLE_H_INCLUDED


#include "../../math/header.h"
#include "../pdspConstants.h"

namespace pdsp {
    /*!
    @cond HIDDEN_SYMBOLS
    */
    class Tunable{
        friend void setReferenceFrequency(float frequency);
        friend void vect_pitchToFreq(float* dest, const float* src, int sizeToProcess);
   
    public:
        static float freqCalculationMultiplier;
        
    };
    
    
    void setReferenceFrequency(float frequency);
    
    
    float p2f (float frequency);
    
    inline_f void vect_pitchToFreq(float* dest, const float* src, int sizeToProcess){
        vect_calculateIncrement(dest, src, Tunable::freqCalculationMultiplier , sizeToProcess);
    }
    /*!
    @endcond
    */
}




#endif  // TUNABLE_H_INCLUDED
