
// OnePole.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_FILTERS_ONEPOLE_H_INCLUDED
#define PDSP_FILTERS_ONEPOLE_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    /*!
    @brief 1 pole high/low pass filter
    
    This is a one pole filters with high and low pass outputs. It is able to process audio-rate and control rate signals so it can also be useful to smooth out control rates, but we advice you to use CRSlew for that.
    */
class OnePole : public Unit {
public:
    OnePole();
    
    /*!
    @brief sets the default filter cutoff and returns the unit ready to be patched.
    @param[in] freq frequency at which the filter operates
    */
    Patchable& set(float freq);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    */
    Patchable& in_signal();
    
    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the frequency at which the filter operates, broadly speaking.
    */  
    Patchable& in_freq();
    
    /*!
    @brief Sets "lpf" as selected output and returns this Unit ready to be patched. This is the default output. This is the  low pass output.
    */ 
    Patchable& out_lpf();
    
    /*!
    @brief Sets "hpf" as selected output and returns this Unit ready to be patched. This is the high pass output.
    */ 
    Patchable& out_hpf();
    

    [[deprecated("this unit has input in hertz, in_cutoff() deprecated, use in_freq() instead")]]
    Patchable& in_cutoff();

private:
    void process(int bufferSize) noexcept override ;
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;
    

    template<bool inputAR, bool cutoffAR>
    void process_audio(const float* &inputBuffer, const float* &cutoffBuffer, const int &bufferSize)noexcept;
    
    inline_f void calculateCoefficient (float wa);

    InputNode input_cutoff;
    InputNode input_signal;
    
    OutputNode output_lpf;
    OutputNode output_hpf;        

    float z1;
    float wa;
    float alpha;
    float halfT;
    float twoSlashT;
    
};
    
}// END NAMESPACE


#endif  // PDSP_FILTERS_ONEPOLE_H_INCLUDED
