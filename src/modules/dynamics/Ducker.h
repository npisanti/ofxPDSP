
// Ducker.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_DUCKER_H_INCLUDED
#define PDSP_MODULE_DUCKER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/envelopes/AHR.h"
#include "../../DSP/utility/DBtoLin.h"
#include "../../DSP/utility/LinToDB.h"
#include "../../DSP/utility/OneMinusInput.h"

namespace pdsp{

/*!
@brief Triggered ducking module.

This module has a trigger input that activate an envelope to duck the signal, is something like a sidechain compressor but with more control.
*/       

class Ducker : public Patchable {
    
public:
    Ducker( bool linkChannels ){ patch(); };
    
    Ducker(){ patch(); };
    Ducker(const Ducker& other){ patch(); };
    Ducker& operator=(const Ducker& other){ return *this; };

    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();
    
    /*!
    @brief Sets "trig" as selected input and returns this module ready to be patched. This input is the trigger input of the ducker. It is not velocity sensitive.
    */   
    Patchable& in_trig();
    
    /*!
    @brief Sets "velocity" as selected input and returns this module ready to be patched. This input is the sensitivity to trigger inputs. Default value is 0.0f (no sensitivity to difference in trigger inputs).
    */   
    Patchable& in_velocity();
    
    /*!
    @brief Sets "ducking" as selected input and returns this module ready to be patched. This is the amount of the gain reduction. Default init value is -20dB. The upper range is limited to 0.0f, the lower to -3000.0f (but usually signal lower than -48.0f are interpreted as ducking up to full silence), see pdsp::DBtoLin .
    */      
    Patchable& in_ducking();
    
    /*!
    @brief Sets "attack" as selected input and returns this module ready to be patched. This input is the attack time of the ducker in milliseconds, how much time the envelope will take to reach the ducking value. Init default time is 50ms.
    */   
    Patchable& in_attack();
    
    /*!
    @brief Sets "hold" as selected input and returns this module ready to be patched. This input is the hold time of the ducker in milliseconds, for how much time the ducking value will be hold. Init default value is 0 ms.
    */   
    Patchable& in_hold();
    
    /*!
    @brief Sets "release" as selected input and returns this module ready to be patched. This input is the release time of the ducker in milliseconds, how much time after the hold time the ducker will tack to return to the original signal value.
    Default time is 100ms.
    */   
    Patchable& in_release();
        
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. I
    */  
    Patchable& out_1();
    
    /*!
    @brief sets the curve of the attack stage the internal envelope, from a smoother linear in dB curve to an harder analog-like curve. By default is 0.0f ( linear in dB attack ).
    @param[in] hardness how much analog-like the analog curve is, value is clampet to the 0.0<-->1.0f range, 
    */
    void setAttackCurve(float hardness); 
 
    /*!
    @brief sets the curve of the release stage of the internal envelope, from a smoother linear in dB curve to an harder analog-like curve. By default is 1.0f ( analog-like releases ).
    @param[in] hardness how much analog-like the analog curve is, value is clampet to the 0.0<-->1.0f range, 
    */
    void setReleaseCurve(float hardness); 
  
    /*!
    @brief returns the envelope signal. This method is thread-safe.
    */  
    float meter_env() const ;
    
private:
    void patch();
    
    PatchNode               input1;
    PatchNode               input2;
    
    DBtoLin                 cleanPart;    
    OneMinusInput           ducking;
        
    AHR                     envelope;
    OneMinusInput           oneMinusEnv;
    Amp                     envAmt;
    
    Amp                     dca1;
    Amp                     dca2;
    
};



} // end namespace


#endif // PDSP_MODULE_DUCKER_H_INCLUDED
