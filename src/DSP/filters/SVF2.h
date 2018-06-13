
// SVF2.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FILTERS_SVF2POLE_H_INCLUDED
#define PDSP_FILTERS_SVF2POLE_H_INCLUDED


#include "../pdspCore.h"

namespace pdsp {

    /*!
    @brief 2pole Virtual Analog State Variable Filter
    
    This is a 2 pole SVF implemented as described in Will Pirke's book "Designing Software Synthesizer Plugins in c++". It has cutoff and resonance input and four different filter outs for the various filter modes.
    */   

class SVF2 : public Unit {
public:
    
    SVF2();
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    */  
    Patchable& in_signal();
    
    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the cutoff frequency (in hertz) at which the filter operates, broadly speaking.
    */      
    Patchable& in_freq();
    
    /*!
    @brief Sets "reso" as selected input and returns this Unit ready to be patched. This is the resonance of the filter.
    */ 
    Patchable& in_reso();

    /*!
    @brief Sets "lpf" as selected output and returns this Unit ready to be patched. This is the default output. This is the  low pass output.
    */ 
    Patchable& out_lpf();
    
    /*!
    @brief Sets "hpf" as selected output and returns this Unit ready to be patched. This is the high pass output.
    */ 
    Patchable& out_hpf();
    
    /*!
    @brief Sets "bpf" as selected output and returns this Unit ready to be patched. This is the band pass output.
    */ 
    Patchable& out_bpf();
    
    /*!
    @brief Sets "notch" as selected output and returns this Unit ready to be patched. This is the band reject output.
    */ 
    Patchable& out_notch();
    
   
    [[deprecated("this unit has input in hertz, in_cutoff() deprecated, use in_freq() instead")]]
    Patchable& in_cutoff();
              
private:
    void process(int bufferSize) noexcept override ;
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;
         
    template<bool cutoffChange, bool resoChange>
    void process_once(const float* &cutoffBuffer, const float* &resoBuffer)noexcept;
   
    template<bool cutoffAR, bool resoAR>
    void process_audio(const float* inputBuffer, const float* cutoffBuffer, const float* &resoBuffer, const int &bufferSize)noexcept;

    InputNode input_cutoff;
    InputNode input_reso;
    
    OutputNode output_lpf;
    OutputNode output_hpf;
    OutputNode output_bpf;
    OutputNode output_bsf;
    
    InputNode input_signal;
    
    float z1_1;
    float z1_2;
    
    float g;
    float Q;
    float R;
    float alpha;
    float alpha0;
    float rho;
    
    float halfT;
    float twoSlashT;
    
};

}




#endif  // PDSP_FILTERS_SVF2POLE_H_INCLUDED
