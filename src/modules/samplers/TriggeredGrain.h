
// TriggeredGrain.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_TRIGGEREDGRAIN_H_INCLUDED
#define PDSP_MODULE_TRIGGEREDGRAIN_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/random/TriggeredRandom.h"
#include "../../DSP/samplers/Sampler.h"
#include "../../DSP/samplers/GrainWindow.h"
#include "../../DSP/control/TriggerGeiger.h"

namespace pdsp{

    /*!
    @brief A stream of really short sampled segments, use many of this to create a "graincloud"
    */     
class TriggeredGrain : public Patchable {

public:

    TriggeredGrain();
    TriggeredGrain(const TriggeredGrain &other);
    TriggeredGrain& operator=(const TriggeredGrain &other);

    /*!
    @brief returns the last processed envelope amplitude value. This method is thread-safe.
    */
    float meter_env() const;
    
    /*!
    @brief returns a value from 0.0f to 1.0f that broadly rapresent the "playhead" of the current sample. This method is thread-safe.
    */ 
    float meter_position() const;
    
    /*!
    @brief returns a value from -1.0f to 1.0f that rapresent the jitter of the grain. This method is thread-safe.
    */ 
    float meter_jitter() const;
    
    /*!
    @brief Sets "trig" as selected input and returns this module ready to be patched. This is the default input. Each time a trigger is received a grain is triggered.
    */ 
    Patchable& in_trig();

    /*!
    @brief Sets "position" as selected input and returns this module ready to be patched. This is the default input. Send a value between 0.0f and 1.0f to control the position of the grains inside the sample.
    */ 
    Patchable& in_position();
    
    /*!
    @brief Sets "length" as selected input and returns this module ready to be patched. Controls the grain length in milliseconds.
    */ 
    Patchable& in_length();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. Controls the grain pitch in semitones.
    */
    Patchable& in_pitch();

    /*!
    @brief Sets "select" as selected input and returns this module ready to be patched. Select the sample to stream from the ones previously added.
    */
    Patchable& in_select();
    
    /*!
    @brief Sets "position_jitter" as selected input and returns this module ready to be patched. Grain position jitter control relative to the sample length. 0.0f = no jitter, 1.0f = position jitter as long as the entire sample lenght.
    */ 
    Patchable& in_position_jitter();
    
    /*!
    @brief Sets "pitch_jitter" as selected input and returns this module ready to be patched. Grain pitch jitter in semitones.
    */ 
    Patchable& in_pitch_jitter();
    
    /*!
    @brief Sets "direction" as selected input and returns this module ready to be patched. A positive value makes the grains play forward, a negative in reverse.
    */ 
    Patchable& in_direction();

    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the stream output.
    */ 
    Patchable& out_signal();
    
    /*!
    @brief Sets "jitter" as selected output and returns this Unit ready to be patched. This output produce a random value between -1.0f and 1.0f that changes each time a grain is retriggered.
    */ 
    Patchable& out_jitter();

    /*!
    @brief adds a pointer to a SampleBuffer to an internal array of SampleBuffer pointers
    @param[in] newSample pointer to a SampleBuffer
    */ 
    void addSample(SampleBuffer* newSample);
    
    /*!
    @brief Sets the SampleBuffer pointer at the given index to a new pointer.
    @param[in] samplePointer pointer to a sample buffer with a loaded file inside
    @param[in] index index of the position of the sample to set inside the sample pointers table
    */ 
    void setSample(SampleBuffer* samplePointer, int index=0);
    
    /*!
    @brief sets the envelope window shape, optionally the resolution of the table.
    @param[in] type window type
    @param[in] window_length window length, if not specified 1024
    */    
    void setWindowType(Window_t type, int window_length=1024 );

private:

    void patch();
    
    PatchNode       trigger_in;
    
    Sampler         grain;
    GrainWindow     window;
    
    TriggeredRandom jitter;
    
    Amp             posJitterAmt;
    Amp             pitchJitterAmt;
  
};

} //END NAMESPACE




#endif // PDSP_MODULE_TRIGGEREDGRAIN_H_INCLUDED
