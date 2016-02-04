
// GrainWindow.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SAMPLERS_GRAINWINDOW_H_INCLUDED
#define PDSP_SAMPLERS_GRAINWINDOW_H_INCLUDED


#include "../pdspCore.h"
#include "SampleBuffer.h"
#include "grain/GrainTable.h"
#include "grain/GrainBased.h"
#include "../interpolators/interpolators.h"

namespace pdsp{

    /*!
    @brief Single trigger envelope with a DSP-window shape, with included amp.

    This is a combination of an amp and a single trigger envelope that use a window shape. It is useful together with Sampler to create granular sampling modules. It uses linear interpolation on the table, for higher resolution choose a bigger table size. Standard table size is 1024.
    */
class GrainWindow : 
    public Unit,
    public GrainBased
{

public:

    GrainWindow();

    /*!
    @brief sets the default envelope length and returns the unit ready to be patched.
    @param[in] grainLengthMs envelope length
    */
    Patchable& set(float grainLengthMs);
    
    /*!
    @brief sets the default envelope length, the envelope window shape, optionally the resolution of the table and returns the unit ready to be patched.
    @param[in] grainLengthMs envelope length
    @param[in] type window type
    @param[in] window_length window length, if not specified 1024
    */
    Patchable& set(float grainLengthMs, Window_t type, int window_length=1024);

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to which the envelope is applied. If there are no triggers on in the in_trig() input and the envelope is already terminated this input is not even processed. 
    */
    Patchable& in_signal();
    
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. You have to patch an out_trig() to this input. The trigger on values received retrigger the envelope.
    */    
    Patchable& in_trig();
    
    /*!
    @brief Sets "length" as selected input and returns this Unit ready to be patched. This input is checked at control rate so it has no sense to patch audio-rate modulations to it. This is the envelope length in milliseconds. Init default value is 200 ms.
    */    
    Patchable& in_length();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the enveloped output.
    */ 
    Patchable& out_signal();
    
    //void setInterpolatorType(Interpolator_t type);
 
    /*!
    @brief sets the envelope window shape, optionally the resolution of the table.
    @param[in] type window type
    @param[in] window_length window length, if not specified 1024
    */    
    void setWindowType(Window_t type, int window_length=1024);

    /*!
    @brief returns the last processed envelope amplitude value. This method is thread-safe.
    */
    float meter_window() const;

private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override {};
    void process (int bufferSize) noexcept override;

    template<bool signalAR, bool triggerAR>
    void process_audio( const float* inputBuffer, const float* triggerBuffer, int bufferSize) noexcept;

    void changeLength( float newLength);
  
    InputNode input_signal;
    InputNode input_trigger;
    InputNode input_length_ms;

    OutputNode output;

    int direction;
    float length;
    float baseInc;
    float basePos;
    float phase;

    GrainTable* grainShape;
    //InterpolatorShell interpolatorShell;
    int grainLength;
    float sampleRate;

    std::atomic<float> windowMeter;

    bool run;
    float inc;
};

     
}





#endif // PDSP_SAMPLERS_GRAINWINDOW_H_INCLUDED
