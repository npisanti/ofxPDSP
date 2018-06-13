
// BasiVerb.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_BASIVERB_H_INCLUDED
#define PDSP_MODULE_BASIVERB_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/oscillators/cheap/CheapTri.h"
#include "../../DSP/delays/Delay.h"
#include "../../DSP/delays/AllPassDelay.h"
#include "../../DSP/filters/OnePole.h"
#include "../../DSP/filters/APF1.h"
#include "../../DSP/utility/BipolarToUnipolar.h"

namespace pdsp{
/*!
@brief Dubby sounding reverb based on Moorer's reverb with added time modulations.

Default values are: time=3.33s, density=0.5, damping=0.5, hi_cut=5000hz, mod_speed=0.2hz, mod_amount=0.8ms. Most of the control values are update once per audio buffer, no crazy control modulations to this reverb.
*/       

class BasiVerb : public Patchable {


    class RT60Calculator : public Unit{
    
    public:
        RT60Calculator();
        ~RT60Calculator();
        
        InputNode input_rt60;
        InputNode input_shape;
        ValueNode delay_times [6];
        ValueNode delay_g [6];
    
    
    private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override;
        void process (int bufferSize) noexcept override;
        
        static const float delay_mult [6];
        static const float delay_min;
        static const float delay_range;
        
    };

public:

    BasiVerb();
    BasiVerb(const BasiVerb& other);
    BasiVerb& operator=(const BasiVerb& other);


    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the mono reverb input.
    */      
    Patchable& in_signal();

    /*!
    @brief Sets "time" as selected input and returns this module ready to be patched. This is the rt60 value, the time the reverb takes to decay 60dB in volume.
    */  
    Patchable& in_time();
    
    /*!
    @brief Sets "density" as selected input and returns this module ready to be patched. This is a number ranging from 0.0f to 1.0f that rapresenty the reverb echo density, and changes the character of the reverb.
    */  
    Patchable& in_density();
    
    /*!
    @brief Sets "damping" as selected input and returns this module ready to be patched. Use value in the 0.0f-1.0f range, higher values make the reverb's tail become darker sooner. 
    */    
    Patchable& in_damping();
    
    /*!
    @brief Sets "hi_cut" as selected input and returns this module ready to be patched. This is the frequency of a internal one pole lowpass filter that makes the reverb darker.
    */      
    Patchable& in_hi_cut();
    
    /*!
    @brief Sets "mod_freq" as selected input and returns this module ready to be patched. This is the frequency of the modulation LFO. Default value is 0.5hz.
    */       
    Patchable& in_mod_freq();
    
    /*!
    @brief Sets "mod_amount" as selected input and returns this module ready to be patched. Mod amount of the reverb internal delays in milliseconds, keep it lower than 0.5f if you don't want to morph your reverb into some kind of seasickness effect. Default value is 0ms (no modulation).
    */       
    Patchable& in_mod_amount();
    

        
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. 
    */  
    Patchable& out_1();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. 
    */  
    Patchable& out_R();

    /*!
    @brief returns a value between 0.0f and 1.0f that rapresent the mod LFO signal. This method is thread-safe.
    */   
    float meter_lfo() const;
    
private:
    void patch();
 
    RT60Calculator  coeffs;
 
    LFOPhasor       phasor;
    CheapTri        LFO;
 
    Delay     delays [6];

    AllPassDelay    APF_1;
    AllPassDelay    APF_2;
 
    PatchNode       input_signal;

    PatchNode       damping_ctrl;
    PatchNode       hi_cut_ctrl;

    PatchNode       output1;
    PatchNode       output2;
    
    OnePole         lpf1;
    OnePole         lpf2;
    
    BipolarToUnipolar lfoOut;
    Amp               modAmt;
    
    APF1              apf1L;
    APF1              apf1R;

    static const float damp_mult [6];    

};
    
    
} // end namespace



#endif // PDSP_MODULE_BASIVERB_H_INCLUDED
