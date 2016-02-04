
// MainDelay.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DELAYS_MAINDELAY_H_INCLUDED
#define PDSP_DELAYS_MAINDELAY_H_INCLUDED


#include "../pdspCore.h"
#include "../interpolators/interpolators.h"

namespace pdsp{

    /*!
    @brief Digital Delay with patchable feedback.
    
    MainDelay is a digital delay, the minumim delay time for this delay is dependent on the audio buffer size, keeping this limitation it is possible to send the output of MainDelay into other Unit and then patch it back to the input, creating external processing chains for the feedback path. It is also adviced to use a OnePole high pass filter before the input set at 10-20hz to leak any DC offset. The max time of MainDelay is settable with setMaxTime() or using the MainDelay( float timeMS ) constructor.
    */
    
class MainDelay : public Unit {
public:

    MainDelay();
    MainDelay( float timeMs);

    /*!
    @brief sets the default delay time value and returns the unit ready to be patched.
    @param[in] timeMs delay time in milliseconds 
    */ 
    Patchable& set( float timeMs);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to be delayed. You can patch the output back to this input to make a feedback path.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "time" as selected input and returns this Unit ready to be patched. This is the delay time and obviously support audio-rate modulation. The time value is clamped to the max delay time of the delay buffer and to the minimum by the audio buffer size time.
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
    
    /*!
    @brief changes the interpolator used for getting values between samples. Using a better interpolator make the delay sound better but it is also more cpu expensive.
    @param[in] interpolatorMode the interpolator type to set
    */     
    void changeInterpolator(Interpolator_t interpolatorMode);
    
    /*!
    @brief returns the time value at the start of the last processed buffer. This method is thread-safe.
    */ 
    float meter_time() const;
    
    //void timeBoundaryEnabled(bool boundaries);
    
private:
    void process(int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void initDelayBuffer();

    void process_once(const float* timeBuffer);
    
    template<bool timeAR>
    void read_audio(float* &outputBuffer, const float* &timeBuffer, const int &bufferSize);
    
    void write_audio(float* &outputBuffer, const int& bufferSize);

    void updateBoundaries();
    
    int     writeIndex;
    float   readIndex;       
    
    InputNode   in_time_ms;
    InputNode   input;
    OutputNode  output;
    
    InterpolatorShell interShell;
    
    float   msToSamplesMultiplier;
    float*  delayBuffer;
    float   maxDelayTimeMs;
    float   sampleRate;
    int     maxDelayTimeSamples;
    float   maxDelayTimeSamples_f;
    
    bool    boundaries;
    
    std::atomic<float>  timeMeter;
};

}





#endif  // PDSP_DELAYS_MAINDELAY_H_INCLUDED
