
// LHDelay.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2018

#ifndef PDSP_UTILITY_LHDELAY_H_INCLUDED
#define PDSP_UTILITY_LHDELAY_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

    /*!
    @brief Utility to delay signal for a certain ms, used in lookahead compressors
    
    */
class LHDelay : public Unit {
public:

    LHDelay();

    LHDelay( float timeMs );

    /*!
    @brief sets the default delay time, and returns the unit ready to be patched.
    @param[in] timeMs delay time in milliseconds 
    */     
    Patchable& set( float timeMs );
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to be delayed. You can patch the output back to this input to make a feedback path.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "time" as selected input and returns this Unit ready to be patched. This is the delay time, with no support to audio rate modulation. The time value is clamped to the max delay time of the delay buffer.
    */  
    Patchable& in_time();
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the delayed signal.
    */       
    Patchable& out_signal();

    /*!
    @brief sets maximum delay time of this delay, The old delay buffer will be deleted and a new will be created, so this is absolutely not thread safe if used in the audio callback.
    @param[in] timeMs new maximum delay time.
    */ 
    void setMaxTime(float timeMs);


private:
    void process(int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void initDelayBuffer();
    void updateBoundaries();
    
        
    InputNode       input;
    InputNode       in_time_ms;
    
    OutputNode      output;
    
    float* delayBuffer;
   
    float msToSamplesMultiplier;
    float maxDelayTimeMs;
    float sampleRate;
    int maxDelayTimeSamples;   
    
    int writeIndex;
    int readIndex;
    int offset;
    
    int silenceSamples;

};

} // pdsp namespace ned


#endif  // PDSP_UTILITY_LHDELAY_H_INCLUDED
