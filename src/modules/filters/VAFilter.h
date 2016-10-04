
// VAFilter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_VAFILTER_H_INCLUDED
#define PDSP_MODULE_VAFILTER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/filters/MultiLadder4.h"
#include "../../DSP/utility/PitchToFreq.h"


namespace pdsp{
    
    /*!
    @brief Multi-mode virtual analog filter, with mono or stereo channels
    */  
       
class VAFilter : public Patchable {
    

public:
    VAFilter(){ patch(); };
    VAFilter(const VAFilter& other){ patch(); };
    VAFilter& operator=(const VAFilter& other){ return *this; };

    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();
    
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_L();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_R();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the filter cutoff control in semitones.
    */      
    Patchable& in_pitch();
    
    
    /*!
    @brief Sets "reso" as selected input and returns this module ready to be patched. This is the filter resonance control.
    */      
    Patchable& in_reso();
    
    
    /*!
    @brief Sets "mode" as selected input and returns this module ready to be patched. This control switches the filter type, there are some static constant in this class that rapresent different filter type values.
    
    Constants for filter types are:
        LowPass24 = 0.0f;
        LowPass12 = 1.0f;
        HighPass24 = 2.0f;
        HighPass12 = 3.0f;
        BandPass24 = 4.0f;
        BandPass12 = 5.0f;
    you can access them in the scope, and patch it like this pdsp::VAFilter::LowPass12 >> filter.in_mode()
    */      
    Patchable& in_mode();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono EQ.
    */  
    Patchable& out_1();
            
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono EQ.
    */  
    Patchable& out_R();
            
            
    static const float LowPass24;;
    static const float LowPass12;
    static const float HighPass24;
    static const float HighPass12;
    static const float BandPass24;
    static const float BandPass12;
            
private:
    void patch ();
    

    PatchNode reso;
    PatchNode mode;
    
    PitchToFreq p2f;
    
    
    MultiLadder4 filterA;
    MultiLadder4 filterB;

    Switch  switchA;
    Switch  switchB;

};

} // pdsp namespace end


#endif // PDSP_MODULE_VAFILTER_H_INCLUDED
