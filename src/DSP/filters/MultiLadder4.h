
// MultiLadder4.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_FILTERS_MULTILADDER4_H_INCLUDED
#define PDSP_FILTERS_MULTILADDER4_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    /*!
    @brief 4 pole Multimode Ladder Virtual Analog Filter
    
    This is a 4 pole Multimode Ladder Filter implemented as described in Will Pirke's book "Designing Software Synthesizer Plugins in c++". It has cutoff and resonance input and six different filter outs for the various filter modes.
    */   
class MultiLadder4 :  public Unit
{
public:

    MultiLadder4();

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    */  
    Patchable& in_signal();
    
    /*!
    @brief Sets "cutoff" as selected input and returns this Unit ready to be patched. This is the frequency at which the filter operates, broadly speaking.
    */      
    Patchable& in_cutoff();
    
    /*!
    @brief Sets "reso" as selected input and returns this Unit ready to be patched. This is the resonance of the filter.
    */ 
    Patchable& in_reso();
    

    /*!
    @brief Sets "lpf4" as selected output and returns this Unit ready to be patched. This is the default output. This is the 4 pole low pass output.
    */ 
    Patchable& out_lpf4();

    /*!
    @brief Sets "lpf2" as selected output and returns this Unit ready to be patched. This is the 2 pole low pass output.
    */     
    Patchable& out_lpf2();
    
    /*!
    @brief Sets "bpf4" as selected output and returns this Unit ready to be patched. This is the 4 pole band pass output.
    */      
    Patchable& out_bpf4();
    
    /*!
    @brief Sets "bpf2" as selected output and returns this Unit ready to be patched. This is the 2 pole band pass output.
    */  
    Patchable& out_bpf2();
    
    /*!
    @brief Sets "hpf4" as selected output and returns this Unit ready to be patched. This is the 4 pole high pass output.
    */      
    Patchable& out_hpf4();
    
    /*!
    @brief Sets "hpf2" as selected output and returns this Unit ready to be patched. This is the 2 pole high pass output.
    */  
    Patchable& out_hpf2();
    
    
private:
    void process(int bufferSize) noexcept override ;
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;

    template<bool cutoffChange, bool resoChange>
    void process_once( const float* cutoffBuffer, const float* resoBuffer)noexcept;

    template<bool cutoffAR, bool resoAR>
    void process_audio(const float* inputBuffer, const float* cutoffBuffer, const float* resoBuffer, int bufferSize) noexcept;


    inline_f void coefficientCalculation(float wa);
    inline_f void calculateAlphaZero(float reso);
 
    InputNode input_cutoff;
    InputNode input_reso;

    OutputNode output_lpf4;
    OutputNode output_lpf2;
    OutputNode output_bpf2;
    OutputNode output_bpf4;
    OutputNode output_hpf4;
    OutputNode output_hpf2;

    InputNode input_signal;

    float g, alpha, beta4, beta3, beta2, beta1, gamma, alpha0;
    float K;

    float z1_1;
    float z1_2;
    float z1_3;
    float z1_4;


    float halfT;
    float twoSlashT;


};

}//END NAMESPACE


#endif  // PDSP_FILTERS_MULTILADDER4_H_INCLUDED
