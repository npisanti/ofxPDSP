
// VAOscillator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_VAOSCILLATOR_H_INCLUDED
#define PDSP_MODULE_VAOSCILLATOR_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/oscillators/antialiased/BLEPSaw.h"
#include "../../DSP/oscillators/antialiased/DPWTri.h"
#include "../../DSP/oscillators/fm/SineFB.h"
#include "../../DSP/oscillators/phazors/PMPhazor.h"
#include "../../DSP/utility/PitchToFreq.h"

#include "../../DSP/oscillators/phazors/PhazorShifter.h"
#include "../../DSP/filters/OnePole.h"


namespace pdsp{
    
/*!
@brief Oscillator with antialiased waveforms, sync in/out and fm input.

*/  

class VAOscillator : public Patchable {
    
public:

    VAOscillator();
    VAOscillator(const VAOscillator &Other);
    VAOscillator& operator=(const VAOscillator &Other);

    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the default input. This is the oscillator pitch input.
    */      
    Patchable& in_pitch();

    /*!
    @brief Sets "pw" as selected input and returns this module ready to be patched. This is the pulse width for the pulse waveform. The input is clamped to (0.0f 1.0f) range. Init default value is 0.5f (square wave).
    */         
    Patchable& in_pw();
    
    /*!
    @brief Sets "sync" as selected input and returns this module ready to be patched. This is the sync input. You have to patch an out_sync() to this. This input is not compatible with out_trig().
    */
    Patchable& in_sync();
    
    /*!
    @brief Sets "saw" as selected output and returns this module ready to be patched. This is the default output. This is the saw waveform output.
    */  
    Patchable& out_saw();
    
    /*!
    @brief Sets "pulse" as selected output and returns this module ready to be patched. This is the pulse waveform output. You control the pulse width the in_shape() input.
    */ 
    Patchable& out_pulse();
    
    /*!
    @brief Sets "triangle" as selected output and returns this module ready to be patched. This is the triangle waveform output.
    */ 
    Patchable& out_triangle();

    /*!
    @brief Sets "sine" as selected output and returns this module ready to be patched. This is the sine waveform output.
    */ 
    Patchable& out_sine();
    
    /*!
    @brief Sets "sync" as selected output and returns this module ready to be patyhed. This is an output with values needed to perform oscillator sync, you patch it to the corrispondent in_sinc() input.
    */ 
    Patchable& out_sync();
    
    /*!
    @brief meters the oscillator pitch. This method is thread-safe.
    */ 
    float meter_pitch();
    
private:
    void patch();
    
    BLEPSaw         saw;
    BLEPSaw         saw2; //for making pulse
    
    DPWTri          triangle;
    SineFB          sine;
    PitchToFreq     p2f;
    PMPhazor        phazor;
    PatchNode       inputShape;
    
    PhazorShifter   shift;
    OnePole         leakDC;
    
    
    
};

} //END NAMESPACE


#endif // PDSP_MODULE_VAOSCILLATOR_H_INCLUDED
