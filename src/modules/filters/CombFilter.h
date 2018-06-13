// CombFilter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_COMBFILTER_H_INCLUDED
#define PDSP_MODULE_COMBFILTER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/delays/Delay.h"
#include "../../DSP/utility/PitchToFreq.h"
#include "../../DSP/utility/FreqToMs.h"


namespace pdsp{
/*!
@brief A comb filter is a delay tuned to a specific pitch frequency (mix it with the dry signal). 
* 
*/       

class CombFilter : public Patchable {

public:
    CombFilter();
    CombFilter(const CombFilter& other);
    CombFilter& operator=(const CombFilter& other);

    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the filter input.
    */      
    Patchable& in_signal();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the tuning of the delay in semitones.
    */  
    Patchable& in_pitch();
    /*!
    @brief Sets "fb" as selected input and returns this module ready to be patched. This is the delay feedback value, mostly it behaves like a resonance parameter.
    */     
    Patchable& in_fb();

    /*!
    @brief Sets "damping" as selected input and returns this module ready to be patched. This is the damping of the delay feedback.
    */  
    Patchable& in_damping();
    
    /*!
    @brief Sets "signal" as selected output and returns this module ready to be patched. This is the default output. This is filter signal output.
    */  
    Patchable& out_signal();


    /*!
    @brief returns the actual pitch value.This method is thread-safe.
    */  
    float meter_pitch() const;

private:
    
    void patch();


    Delay delay;
    PitchToFreq p2f;
    FreqToMs    f2ms;


};


}

#endif //PDSP_MODULE_COMBFILTER_H_INCLUDED
