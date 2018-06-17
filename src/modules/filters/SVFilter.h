
// SVFilter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2018

#ifndef PDSP_MODULE_SVFILTER_H_INCLUDED
#define PDSP_MODULE_SVFILTER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/filters/SVF2.h"
#include "../../DSP/utility/PitchToFreq.h"


namespace pdsp{
    
    /*!
    @brief Multi-mode virtual analog state variable filter, multichannel.
    */  
    
class SVFilter : public Patchable {



/*!
    @cond HIDDEN_SYMBOLS
*/    
class Submodule : public Patchable{
public:
    Submodule();
private:
    SVF2 filter;
    Switch fswitch;
};
/*!
    @endcond
*/


public:
    SVFilter(){ patch(); };
    SVFilter(const SVFilter& other){ patch(); };
    SVFilter& operator=(const SVFilter& other){ return *this; };
    ~SVFilter();

    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the filter input.
    */      
    Patchable& in_signal();
    
    /*!
    @brief Sets "signal" as selected output and returns this module ready to be patched. This is the default output. This is filter signal output.
    */  
    Patchable& out_signal();    
    
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the filter cutoff control in semitones.
    */      
    Patchable& in_pitch();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the filter cutoff control in semitones.
    */      
    Patchable& in_cutoff();
    
    
    /*!
    @brief Sets "reso" as selected input and returns this module ready to be patched. This is the filter resonance control.
    */      
    Patchable& in_reso();
    
    /*!
    @brief Sets "mode" as selected input and returns this module ready to be patched. This control switches the filter type, there are some static constant in this class that rapresent different filter type values. Default is 0, a 12db/oct Low Pass Filter.
    
    Constants for filter types are:
        LowPass = 0.0f;
        BandPass = 1.0f;
        HighPass = 2.0f;
        Notch = 3.0f;
    you can access them in the pdsp::SVFilter scope, and patch it like this pdsp::SVFilter::LowPass12 >> filter.in_mode()
    */      
    Patchable& in_mode();

    
    /*!
    @brief sets the number of different inputs this module can process. Be sure to set the channels before doing any patching. You can access channels with in/out methods, using a string with their number as input, for example in("1") or out("4"), or by using ini(int ch) or outi(int ch) methods.
    @param[in] size number of channels
    */        
    void channels( int size );    
    
    /*!
    @brief To use the selected channel as input/output for the operation.
    @param[in] ch channel index
    */  
    Patchable& operator[]( const int & ch );
        


    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */     
    Patchable& in_1();
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */      
    Patchable& in_L();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */     
    Patchable& in_R();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */  
    Patchable& out_1();
            
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2 and activate "0" and "1" outputs.
    */  
    Patchable& out_R();
            
            
    /*!
    @brief Returns the cutoff pitch value.
    */      
    float meter_cutoff() const;        
    
            
    static const float LowPass;;
    static const float BandPass;
    static const float HighPass;
    static const float Notch;
            
    
private:
    void patch ();
    
    PatchNode   reso;
    PatchNode   mode;
    PitchToFreq p2f;
    
    std::vector<Submodule*> submodules;

};

} // pdsp namespace end


#endif // PDSP_MODULE_SVFILTER_H_INCLUDED
