
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
    @brief Multi-mode virtual analog state variable filter. Multichannel.
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
    @brief Allocate a number of channels for processing different inputs. This is automatically called if you query for a channel outside the allocated range. You can access different channels with the ch() method.
    @param[in] size number of channels
    */        
    void channels( size_t size );    

    /*!
    @brief Uses the selected channel as input/output for the patching operation.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );
            
    /*!
    @brief Returns the cutoff pitch value.
    */      
    float meter_cutoff() const;        
    
            
    static const float LowPass;;
    static const float BandPass;
    static const float HighPass;
    static const float Notch;

/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("operator[] deprecated, use the ch( int index ) method instead")]]    
    Patchable& operator[]( size_t index );
    
    [[deprecated("in_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_0();
    
    [[deprecated("in_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_1();
    
    [[deprecated("out_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_0();
    
    [[deprecated("out_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_1();
    
    [[deprecated("in_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_L();
    
    [[deprecated("in_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_R();
    
    [[deprecated("out_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_L();
    
    [[deprecated("out_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_R();
/*!
    @endcond
*/
     
    
private:
    void patch ();
    
    PatchNode   reso;
    PatchNode   mode;
    PitchToFreq p2f;
    
    std::vector<Submodule*> submodules;

};

} // pdsp namespace end


#endif // PDSP_MODULE_SVFILTER_H_INCLUDED
