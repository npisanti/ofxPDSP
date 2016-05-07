
// UsesSlew.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_HELPERS_USESSLEW_H_INCLUDED
#define PDSP_HELPERS_USESSLEW_H_INCLUDED

#include "../pdspCore.h"
#include "../../math/header.h"

namespace pdsp{

    /*!
    @brief Slewing behavior.
    
    Classes that inherits from UsesSlew have some slewing of signal (usually control signals). Think about it as them implementing "portamento" between values.
    */   

class UsesSlew{

public:

    /*!
    @brief sets the slew time in milliseconds
    @param[in] slewTimeMs the time for slewing
    @param[in] mode the mode of slewing, if not given is Time. 
    
    The two available slew mode are Time and Rate. In Time mode you set the time for slewing to a value to another and it is not important how the values are distant. In Rate mode you set the time it takes for the slew to change of a given amount. You set this amount with setSlewRateModeReference(), the standard is 12.0f .
    */ 
    void setSlewTime(float slewTimeMs, SlewMode_t mode = Time);
    
    /*!
    @brief sets the slew time to 0.0f
    */    
    void deactivateSlew();
    
    /*!
    @brief sets the reference value for Rate mode slewing.
    @param[in] valueReference calibration value for Rate mode
    
    sets the reference value for Rate mode slewing. For example if we are slewing some pitch control, slew time is 50ms and this value is 7.0f the algorithm will more or less take 50ms to travel 7 semitones. I say "more or less" as the algorhitm emulates analog capacitors so it is slower when it approaches the destination value.
    */   
    void setSlewRateModeReference(float valueReference);
  
/*!
    @cond HIDDEN_SYMBOLS
*/
        
    void setInitValue( float value );

protected:

    UsesSlew();
    
    void valueChange( const float &valueDest ) noexcept;
    void runSlewBlock(float* &buffer, const int &start, const int &stopExclusive) noexcept;

    void prepareSlew(float sampleRate, float initValue);

    float slewInitValue;
    std::atomic<float> slewLastValue;
    float slewTimeMod;
    
    float sampleRate;
    float slewTimeMs;
    float slewTCO;
    
    SlewMode_t slewMode;
    float slewTimeRangeMultiplier;
    
    float slewValueStart;
    float slewValueDest;
    float slewAccumulator;
    float slewCoeff;
    float slewOffset;
    float slewValueInterval;
    bool slewRun;
/*!
    @endcond
*/

};


}//END NAMESPACE

#endif //PDSP_HELPERS_USESSLEW_H_INCLUDED
