
// TriggeredRandom.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_RANDOM_TRIGGEREDRANDOM_H_INCLUDED
#define PDSP_RANDOM_TRIGGEREDRANDOM_H_INCLUDED

#include <time.h>
#include "../pdspCore.h"

namespace pdsp{
    /*!
    @brief Random values clocked by an external trigger.
    
    This units outputs a random value that ranges from -1.0f to 1.0f. This value is changed each time a trigger on value is received by the in_trig() input.
    */ 
    
class TriggeredRandom : public Unit{

public:
    TriggeredRandom();
    
    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. This is the default input. You have to connect an out_trig() output to this input.  Each time a trigger on value is received the output value of this Unit changes, it is like a random sample & hold generator.
    */ 
    Patchable& in_trig();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the stepped random output.
    */ 
    Patchable& out_signal();

    /*!
    @brief returns the last triggered value.
    */
    float meter_output() const;
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources () override;
    void process (int bufferSize) noexcept override ;

    template<bool smoothed>
    void process_audio (int bufferSize) noexcept ;

    OutputNode output;
    InputNode input_trig;

    float sampled;
    
    std::atomic<float> meter;
};


}

#endif  // PDSP_RANDOM_TRIGGEREDRANDOM_H_INCLUDED
