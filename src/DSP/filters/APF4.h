
// APF4.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FILTERS_APF4_H_INCLUDED
#define PDSP_FILTERS_APF4_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    /*!
    @brief 4pole AllPass Filter
    
    This is a 4pole AllPass filter with feedback, useful to implement phaser effects. This filter operates only on audio-rate signals.
    */
class APF4 :  public Unit
{
public:
    APF4();
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    */  
    Patchable& in_signal();

    /*
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the frequency at which the first APF operates, this frequency is multiplied by a value based on in_spread for the other poles. 
    */
    Patchable& in_freq();

    /*
    @brief Sets "feedback" as selected input and returns this Unit ready to be patched. This is the filter feedback amount.
    */
    Patchable& in_feedback();
    
    /*
    @brief Sets "spread" as selected input and returns this Unit ready to be patched. The value of the frequency of each pole is equal to the frequency of the pole before it plus spread multiplied by the base frequency. So at 0.0f all the pole have the same frequency, at 1.0f the poles form an harmonic series, etc etc
    */    
    Patchable& in_spread();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the filter output.
    */ 
    Patchable& out_signal();

private:
    void process(int bufferSize) noexcept override ;
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;

    template<bool freqChange, bool fbChange, bool spreadChange>
    void process_once( const float* freqBuffer, const float* fbBuffer, const float* spreadBuffer)noexcept;

    template<bool freqAR, bool fbAR, bool spreadAR>
    void process_audio(const float* inputBuffer, const float* freqBuffer, const float* fbBuffer, const float* spreadBuffer, const int &bufferSize)noexcept;
 
    InputNode   input_signal;
    InputNode   input_freq;
    InputNode   input_fb;
    InputNode   input_spread;
    
    OutputNode  output;    

    float x_z1_0;
    float x_z1_1;
    float x_z1_2;
    float x_z1_3;
    
    float y_z1_0;
    float y_z1_1;
    float y_z1_2;
    float y_z1_3;
    
  
    float fb;
    float fb_z1;
  
    float alphaMultiplier;
    float sampleRate;

    ALIGNPRE float alpha [4] ALIGNPOST;
    ALIGNPRE float freqMult [4] ALIGNPOST;
};

}//END NAMESPACE




#endif  // PDSP_FILTERS_APF4_H_INCLUDED
