
// LFO.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_LFO_H_INCLUDED
#define PDSP_MODULE_LFO_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/oscillators/cheap/CheapSaw.h"
#include "../../DSP/oscillators/cheap/CheapSine.h"
#include "../../DSP/oscillators/cheap/CheapTri.h"
#include "../../DSP/oscillators/cheap/CheapPulse.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/oscillators/phasors/ClockedPhasor.h"
#include "../../DSP/filters/OnePole.h"
#include "../../DSP/random/TriggeredRandom.h"

namespace pdsp{
    
    /*!
    @brief non-clocked LFO
    */   
    
class LFO : public Patchable {
    
public:

    LFO();
    LFO(const LFO &Other);
    LFO& operator=(const LFO &Other);

    /*!
    @brief Sets "freq" as selected input and returns this module ready to be patched. This is the default input. This is the oscillator frequency input in hertz. Init default value is 0.5 hertz.
    */      
    Patchable& in_freq();
    
    /*!
    @brief Sets "retrig" as selected input and returns this module ready to be patched. Connect a "trig" output to this input to activate retriggering of the LFO on trigger on values.
    */  
    Patchable& in_retrig();
    
    /*!
    @brief Sets "phase_start" as selected input and returns this module ready to be patched. This is the starting value of the phasor on retrigger. It is updated only at control-rate.
    */
    Patchable& in_phase_start();
    
    /*!
    @brief Sets "triangle" as selected output and returns this module ready to be patched. This is the default output. This is the triangle waveform output.
    */     
    Patchable& out_triangle();

    /*!
    @brief Sets "sine" as selected output and returns this module ready to be patched. This is the sine waveform output.
    */      
    Patchable& out_sine();
    
    /*!
    @brief Sets "sine" as selected output and returns this module ready to be patched. This is the saw waveform output (rising saw).
    */        
    Patchable& out_saw();
    
    /*!
    @brief Sets "square" as selected output and returns this module ready to be patched. This is the square waveform output.
    */    
    Patchable& out_square();
    
    /*!
    @brief Sets "sample_and_hold" as selected output and returns this module ready to be patched. This is the sample & hold waveform output.
    */        
    Patchable& out_sample_and_hold();
    
    /*!
    @brief Sets "random" as selected output and returns this module ready to be patched. This is random output.
    */ 
    Patchable& out_random();
    
    
private:
    void patch();
    
    
    PatchNode       speed;
    LFOPhasor       phasorFree;

    
    CheapSaw            saw;
    CheapPulse          square;
    CheapSine           sine;
    CheapTri            triangle;
    TriggeredRandom     rnd;
    
    OnePole             randomSlew;
    OnePole             randomSnHSlew;
    OnePole             squareSlew;
    OnePole             sawSlew;

    
};

} //END NAMESPACE


#endif // PDSP_MODULE_LFO_H_INCLUDED
