
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
    @brief Multi-mode virtual analog ladder filter, with mono or stereo channels
    */  
       
class VAFilter : public Patchable {
    

public:
    VAFilter(){ patch(); };
    VAFilter(const VAFilter& other){ patch(); };
    VAFilter& operator=(const VAFilter& other){ return *this; };
    ~VAFilter();
   
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
    @brief Sets "mode" as selected input and returns this module ready to be patched. This control switches the filter type, there are some static constant in this class that rapresent different filter type values. Default is 0, a 24db/oct Low Pass Filter.
    
    Constants for filter types are:
        LowPass24 = 0.0f;
        LowPass12 = 1.0f;
        HighPass24 = 2.0f;
        HighPass12 = 3.0f;
        BandPass24 = 4.0f;
        BandPass12 = 5.0f;
        Notch = 6.0f;
    you can access them in the pdsp::VAFilter scope, and patch it like this pdsp::VAFilter::LowPass12 >> filter.in_mode()
    */      
    Patchable& in_mode();
    
    /*!
    @brief sets the number of different inputs this module can process. Be sure to set the channels before doing any patching. You can access channels with in/out methods, using a string with their number as input, for example in("1") or out("4"), or by using ini(int ch) or outi(int ch) methods.
    @param[in] size number of channels
    */        
    void channels( int size );    
    
    /*!
    @brief Returns the input of the given channels.
    @param[in] ch channel index
    */      
    Patchable& ini( int ch );

    /*!
    @brief Returns the output of the given channel.
    @param[in] ch channel index
    */  
    Patchable& outi( int ch );
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */     
    Patchable& in_1();
    
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. If channel is less than 2, sets channels() to 2.
    */      
    Patchable& in_L();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */     
    Patchable& in_R();
    

    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_1();
            
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. If channel is less than 2, sets channels() to 2.
    */  
    Patchable& out_R();
            
    /*!
    @brief Returns the cutoff pitch value.
    */      
    float meter_cutoff() const;        
    
            
    static const float LowPass24;;
    static const float LowPass12;
    static const float HighPass24;
    static const float HighPass12;
    static const float BandPass24;
    static const float BandPass12;
            
    
private:
    void patch ();
    
    PatchNode   reso;
    PatchNode   mode;
    PitchToFreq p2f;
    
    std::vector<MultiLadder4*> filters;
    std::vector<Switch*> switches;

};

} // pdsp namespace end


#endif // PDSP_MODULE_VAFILTER_H_INCLUDED
