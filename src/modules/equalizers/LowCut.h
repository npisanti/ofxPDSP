
// LowCut.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_LOWCUT_H_INCLUDED
#define PDSP_MODULE_LOWCUT_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/filters/OnePole.h"


namespace pdsp{
    
    /*!
    @brief 12 dB Low Cut (aka High-Pass filter). Non-resonant.
    */  
       
class LowCut : public Patchable {
    

public:
    LowCut(){ patch(); };
    LowCut(const LowCut& other){ patch(); };
    LowCut& operator=(const LowCut& other){ return *this; };

    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();

    /*!
    @brief Sets "freq" as selected input and returns this module ready to be patched. This is the frequency at which the EQ operates.
    */      
    Patchable& in_freq();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono EQ.
    */  
    Patchable& out_1();
            
            
private:
    void patch ();
    
    PatchNode   freq;

    PatchNode   input0;
    PatchNode   input1;
    
    OnePole hp0a;
    OnePole hp0b;
    OnePole hp1a;
    OnePole hp1b;
    
};

} // pdsp namespace end


#endif // PDSP_MODULE_LOWCUT_H_INCLUDED
