
// APF1.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FILTERS_APF1_H_INCLUDED
#define PDSP_FILTERS_APF1_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    /*!
    @brief 1pole AllPass Filter
    
    This is a 1 pole AllPass filter with feedback, useful to implement phaser effects or reverbs. This filter operates only on audio-rate signals.
    */
class APF1 :  public Unit
{
public:
    APF1();
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to filter.
    
    */  
    Patchable& in_signal();

    /*!
    @brief Sets "freq" as selected input and returns this Unit ready to be patched. This is the frequency at which the first APF operates, this frequency is multiplied by a value based on in_spread for the other poles. 
    
    */
    Patchable& in_freq();

    /*!
    @brief Sets "feedback" as selected input and returns this Unit ready to be patched. This is the filter feedback amount.
    
    */
    Patchable& in_feedback();
    

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the filter output.
    
    */ 
    Patchable& out_signal();

private:
    void process(int bufferSize) noexcept override ;
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;

    template<bool freqChange, bool fbChange>
    void process_once( const float* freqBuffer, const float* fbBuffer)noexcept;

    template<bool freqAR, bool fbAR>
    void process_audio(const float* inputBuffer, const float* freqBuffer, const float* fbBuffer, const int &bufferSize)noexcept;
 
    InputNode   input_signal;
    InputNode   input_freq;
    InputNode   input_fb;
    
    OutputNode  output;    

    float x_z1_0;
    float y_z1_0;    
  
    float fb;
    float fb_z1;
  
    float sampleRate;
    float alpha;
    float alphaMultiplier;

};

}//END NAMESPACE




#endif  // PDSP_FILTERS_APF1_H_INCLUDED
