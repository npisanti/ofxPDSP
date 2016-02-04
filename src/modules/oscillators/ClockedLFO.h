
// ClockedLFO.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_CLOCKEDLFO_H_INCLUDED
#define PDSP_MODULE_CLOCKEDLFO_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/oscillators/cheap/CheapSaw.h"
#include "../../DSP/oscillators/cheap/CheapSine.h"
#include "../../DSP/oscillators/cheap/CheapTri.h"
#include "../../DSP/oscillators/cheap/CheapPulse.h"
#include "../../DSP/oscillators/phazors/LFOPhazor.h"
#include "../../DSP/oscillators/phazors/ClockedPhazor.h"
#include "../../DSP/filters/OnePole.h"
#include "../../DSP/random/TriggeredRandom.h"

namespace pdsp{

    
    /*!
    @brief LFO clocked to global tempo and synchronized to transport
    */   
    

class ClockedLFO : public Patchable {
    
public:

    ClockedLFO();
    ClockedLFO(const ClockedLFO &Other);
    ClockedLFO& operator=(const ClockedLFO &Other);

    /*!
    @brief sets the default time division and phase offset values and returns the module ready to be patched.
    @param[in] division division time, for example 4.0f is one quarter/cycle and 0.25f means 4 bars/cycle
    @param[in] phaseOffset phase offset at retrigger or in relation to the given division
    */
    Patchable& set(float division, float phaseOffset);
    
    /*!
    @brief Sets "division" as selected input and returns this module ready to be patched. This is the default input. This is the time division, higher is faster, for example 16.0f means 1/16th and 0.5f means two bars/cycle. It is updated only at control-rate.
    */
    Patchable& in_division();
    
    /*!
    @brief Sets "retrig" as selected input and returns this module ready to be patched. Connect a "trig" output to this input to activate manual retriggering of the phazor, otherwise the phasor is synced to the transport
    */  
    Patchable& in_retrig();
    
    /*!
    @brief Sets "phase_offset" as selected input and returns this module ready to be patched. This is the offset of the phazor at retrigger or in relation to the given division. It is updated only at control-rate.
    */
    Patchable& in_phase_offset();    
    
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
    

private:
    void patch();
    
    ClockedPhazor   phazorClocked;

    CheapSaw            saw;
    CheapPulse          square;
    CheapSine           sine;
    CheapTri            triangle;
    TriggeredRandom     rnd;
    
    OnePole             randomSnHSlew;
    OnePole             squareSlew;
    OnePole             sawSlew;

    
};

} //END NAMESPACE


#endif // PDSP_MODULE_CLOCKEDLFO_H_INCLUDED
