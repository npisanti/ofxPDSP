
// Sampler.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SAMPLERS_SAMPLER_H_INCLUDED
#define PDSP_SAMPLERS_SAMPLER_H_INCLUDED

#include "../pdspCore.h"
#include "SampleBuffer.h"

namespace pdsp {
    /*!
    @brief plays SampleBuffer

    This class plays SampleBuffers, it can change the sample pitch, change the start position, play in revers, and select a different sample when is triggered with in_select().
    */


class Sampler : public Unit{

public:
    Sampler();
    
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. This is the default input. Each time a trigger on value is received in_select(), in_start() and in_direction() are processed, the sample is selected and retriggered from the in_start() value and played in the in_direction() direction.
    */    
    Patchable& in_trig();
    
    /*!
    @brief Sets "pitch" as selected input and returns this Unit ready to be patched. This input change the sample pitch (and speed).
    */    
    Patchable& in_pitch();
    
    /*!
    @brief Sets "select" as selected input and returns this Unit ready to be patched. This input change the sample selected from the one added to this Sampler.
    */  
    
    Patchable& in_select();
    
    /*!
    @brief Sets "start" as selected input and returns this Unit ready to be patched. This input change the starting position of the sample playing, from 0.0f (plays from start) to 1.0f (plays from the last sample). 0.0f is the standard value. If you are not hearing any sound probably the start position is not correct. Remember to set it to 1.0f if you want to play the sample backwards (-1.0f >> in_direction() ).
    */  
    Patchable& in_start();
    
    /*!
    @brief Sets "start_mod" as selected input and returns this Unit ready to be patched. This input change how much the dynamic of the trigger will change the start position, lower dynamics will offset the position away from the start (softening the sound). Default value is 0.0f (no trigger influence on start position).
    */
    Patchable& in_start_mod();

    /*!
    @brief Sets "direction" as selected input and returns this Unit ready to be patched. A positive value will set the sampler to play forward and a negative value will set it to play in reverse. Remember to patch a value greater than 0.0f to in_start() if you want to play in reverse.
    */  
    Patchable& in_direction();
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the sampler output.
    */ 
    Patchable& out_signal();

    /*!
    @brief adds a pointer to a SampleBuffer to an internal array of SampleBuffer pointers
    @param[in] newSample pointer to a SampleBuffer
    @param[in] channel selelect channel, usually 0 is left and 1 right, if not given 0 is used
    */ 
    void addSample(SampleBuffer* newSample, int channel=0);
    
    /*!
    @brief Sets the SampleBuffer pointer at the given index to a new pointer.
    @param[in] newSample pointer to a SampleBuffer
    @param[in] index sample index
    @param[in] channel selelect channel, usually 0 is left and 1 right, if not given 0 is used
    */ 
    bool setSample(SampleBuffer* newSample, int index, int channel=0);

    /*!
    @brief returns a value from 0.0f to 1.0f that broadly rapresent the "playhead" of the current sample. This method is thread-safe.
    */ 
    float meter_position() const;

private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override {};
    void process (int bufferSize) noexcept override;

    template<bool pitchModChange>
    void process_once( const float* pitchModBuffer)noexcept;

    template<bool pitchModAR, bool triggerAR>
    void process_audio( const float* pitchModBuffer, const float* triggerBuffer, int bufferSize)noexcept;

    void selectSample( int n, int bufferSize, float trigger) noexcept;
    
    InputNode input_trig;
    InputNode input_pitch_mod;
    InputNode input_select;
    InputNode input_start;
    InputNode input_direction;
    InputNode input_start_mod;
    OutputNode output;
    
    float readIndex;
    float inc;
    SampleBuffer* sample;
    int channel;
    int sampleIndex;
    
    std::vector<SampleBuffer*> samples;
    
    float incBase;
    float direction;
    
    std::atomic<float> positionMeter;
    float positionDivider;
    
    std::vector<int> channels;
    
    const float* selectBuffer;
    int selectState;
    
    const float* startBuffer;    
    int startState;

    const float* startModBuffer;    
    int startModState;

};


}



#endif  // PDSP_SAMPLERS_SAMPLER_H_INCLUDED
