
// Delay.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DELAYS_DAMPEDDELAY_H_INCLUDED
#define PDSP_DELAYS_DAMPEDDELAY_H_INCLUDED


#include "../pdspCore.h"
#include "../interpolators/interpolators.h"


namespace pdsp{

    /*!
    @brief Digital Delay with a low pass filter in the feedback path.
    
    Delay is a delay with a one pole low pass filter inside the feedback path, controllable with the in_damping() input. The feedback is controllable with the in_feedback(). Both the values from in_feedback() and in_damping() are used just once per buffer, so it has really no sense to patch Units running at audio rate to them. in_time() instead supports audio-rate modulation.  The max time of Delay is settable with setMaxTime() or using the Delay( float timeMS ) constructor.
    */
class Delay : public Unit {
public:

    Delay();

    Delay( float timeMs);


    /*!
    @brief sets the default delay time, optionally the default feedback and damping values and returns the unit ready to be patched.
    @param[in] timeMs delay time in milliseconds 
    @param[in] feedback feedback value, if not given 0.0f
    @param[in] damping damping value, if not given 0.0f
    */     
    Patchable& set( float timeMs, float feedback=0.0f, float damping=0.0f);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to be delayed. You can patch the output back to this input to make a feedback path.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "time" as selected input and returns this Unit ready to be patched. This is the delay time and obviously support audio-rate modulation. The time value is clamped to the max delay time of the delay buffer and to the minimum by the audio buffer size time.
    */  
    Patchable& in_time();
    
    /*!
    @brief Sets "feedback" as selected output and returns this Unit ready to be patched. This input is the feedback amount of delay and supports audio-rate modulation.
    */      
    Patchable& in_feedback();
    
    /*!
    @brief Sets "damping" as selected output and returns this Unit ready to be patched. This input control how much the signal in the delay feedback path will be filtered and supports audio-rate modulation. Default value is 0.0f and the value are clipped in the 0.0f - 0.99f range.
    */          
    Patchable& in_damping();    
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the delayed signal.
    */       
    Patchable& out_signal();
    
    /*!
    @brief sets maximum delay time of this delay, The old delay buffer will be deleted and a new will be created, so this is absolutely not thread safe if used in the audio callback.
    @param[in] timeMs new maximum delay time.
    */ 
    void setMaxTime(float timeMs);
    
    //void timeBoundaryEnabled(bool boundaries);
    
    /*!
    @brief changes the interpolator used for getting values between samples. Using a better interpolator make the delay sound better but it is also more cpu expensive.
    @param[in] interpolatorMode the interpolator type to set
    */ 
    void changeInterpolator(Interpolator_t interpolatorMode);
    
    /*!
    @brief returns the time value at the start of the last processed buffer. This method is thread-safe.
    */ 
    float meter_time() const;


private:
    void process(int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void initDelayBuffer();
    
    InputNode       input;
    InputNode       in_time_ms;
    InputNode       input_damping;
    InputNode       input_feedback;
    
    OutputNode      output;

    float msToSamplesMultiplier;
    float* delayBuffer;
    float maxDelayTimeMs;
    float sampleRate;
    int maxDelayTimeSamples;
    float maxDelayTimeSamples_f;
    
    float feedback;


    void setFeedback(float feedback);
    void setDamping(float hiDamp);

    template<bool timeChange, bool fbChange, bool dampChange>    
    void process_once(const float* timeBuffer, const float* fbBuffer, const float* dampBuffer )noexcept;
    
    template<bool inputAR, bool timeAR, bool fbAR, bool dampAR>
    void process_audio(const float* inputBuffer, const float* timeBuffer, const float* fbBuffer, const float* dampBuffer, int bufferSize )noexcept;

    void updateBoundaries();
    
    int writeIndex;
    float readIndex;

    bool boundaries;
    
    InterpolatorShell interShell;
    
    float g;
    float gLPF;
    float z1;
    
    std::atomic<float> timeMeter;
};

} // pdsp namespace ned


#endif  // PDSP_DELAYS_DAMPEDDELAY_H_INCLUDED
