
// FMOperator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_FMOPERATOR_H_INCLUDED
#define PDSP_MODULE_FMOPERATOR_H_INCLUDED

#include "../../DSP/pdspCore.h"

#include "../../DSP/oscillators/fm/SineFB.h"
#include "../../DSP/oscillators/phazors/PMPhazor.h"
#include "../../DSP/utility/PitchToFreq.h"



namespace pdsp{

    /*!
    @brief FM operator based on a wavetable sine.
    */  

class FMOperator : public Patchable {
    
public:

    FMOperator();
    FMOperator(const FMOperator &Other);
    FMOperator& operator=(const FMOperator &Other);


    /*!
    @brief Sets "fm" as selected input and returns this module ready to be patched. This is the default input. This is the operator fm input, we say "FM" but it's really phase-modulation. I suggest keeping the total input signal in the [-4.0f 4.0f] range.
    */    
    Patchable& in_fm();

    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the operator pitch input.
    */      
    Patchable& in_pitch();

    /*!
    @brief Sets "ratio" as selected input and returns this module ready to be patched. This is the operator ratio in respect to in_pitch()
    */             
    Patchable& in_ratio();
    
    /*!
    @brief Sets "fb" as selected input and returns this module ready to be patched. This is the operator FM-feedback input.
    */          
    Patchable& in_fb();
    
    /*!
    @brief Sets "sync" as selected input and returns this module ready to be patched. This is the operator oscillator sync input, you have to patch an out_sync to this for slaving this operator to anoter oscillator clock.
    */  
    Patchable& in_sync();  
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the operator waveform output.
    */ 
    Patchable& out_signal();   

    /*!
    @brief Sets "sync" as selected output and returns this Unit ready to be patched. This is the operator oscillator sync output, you can connect it to an in_sync() for slaving another oscillator to this.
    */ 
    Patchable& out_sync();    

    /*!
    @brief meters the oscillator pitch. This method is thread-safe.
    */ 
    float meter_pitch();
    
private:
    void patch();

    SineFB          sine;
    PitchToFreq     p2f;
    Amp             ratioMult;
    PMPhazor        phazor;
    
};

} //END NAMESPACE


#endif // PDSP_MODULE_FMOPERATOR_H_INCLUDED
