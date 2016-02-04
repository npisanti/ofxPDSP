
// Panner.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_PANNER_H_INCLUDED
#define PDSP_MODULE_PANNER_H_INCLUDED

#include "../../DSP/pdspCore.h"


namespace pdsp{

    /*!
    @brief Pans a signal between two outputs.
    */  
    
class Panner : public Patchable {
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    class PannerFormula : public Formula{
         
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;

    };

    class SinFormula : public Formula{
         
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;

    };

    class CosFormula : public Formula{
         
        float formula(const float &x) noexcept override;
        void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;

    };
/*!
    @endcond
*/
public:
    Panner();
    Panner(const Panner& other);
    Panner& operator=(const Panner& other);

    //Panner(Panner&& other);
    //Panner& operator=( Panner&& other);
    //Panner& operator=( Panner&& other);

    /*!
    @brief meter the pan value. This method is thread-safe.
    */   
    float meter_pan_input();
    
    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the signal to pan.
    */      
    Patchable& in_signal();
    
    /*!
    @brief Sets "pan" as selected input and returns this module ready to be patched. This input is clamped to [-1.0f 1.0f] where -1 is channel0/left and 1 is channel1/right
    */   
    Patchable& in_pan();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel.
    */  
    Patchable& out_1();
        
    
private:
    void patch();
 
    PatchNode       input;
 
    PannerFormula   panning_step1;
    SinFormula      panning_step2L;
    CosFormula      panning_step2R;
        
    Amp             amp1;
    Amp             amp2;
    

};
    
    
    
} // end namespace

#endif // PDSP_MODULE_PANNER_H_INCLUDED
