
// Oscillator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_OSCILLATOR_H_INCLUDED
#define PDSP_OSC_OSCILLATOR_H_INCLUDED

#include "../../pdspCore.h"

namespace pdsp{

   /*!
    @brief Abstract class to implement oscillators
    
    This is a abstract class to rapresent oscillators, with a phase input. You usually have just to implement the oscillate() method
    */

class Oscillator : public Unit{
public:

    Oscillator();
    
    
    /*!
    @brief Sets "phase" as selected input and returns this Unit ready to be patched. This is the default input. You have to patch a "phase" output to this input, usuall taken from some kind of phazor like PMPhazor, LFOPhazor or ClockedPhazor
    */     
    Patchable& in_phase();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the oscillator waveform output.
    */ 
    Patchable& out_signal();

    /*!
    @brief Returns the first value of the last processed output. Usually is more useful to track when the oscillator is running as LFO. This method is thread-safe.
    */     
    float meter_output() const;

protected:
    /*!
    @brief method to implement for preparing oscillator to play, overriding it is not mandatory
    @param[in] sampleRate sample rate, already multiplied if the oscillator is oversampling
    */ 
    virtual void prepareOscillator(double sampleRate);
    
    /*!
    @brief method to implement for cleaning up on resource release, overriding it is not mandatory
    */   
    virtual void releaseOscillator();
    
    /*!
    @brief the oscillator DSP method to implement
    @param[out] outputBuffer buffer to fill with waveform output
    @param[in] phaseBuffer buffer for the phase values
    @param[in] bufferSize buffersize, already multipled if oversampling 
    */     
    virtual void oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept;

private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources() override;
    void process(int bufferSize) noexcept override;

    InputNode input_phase;
    OutputNode output;
    
    std::atomic<float> meter;
};

}





#endif  // PDSP_OSC_OSCILLATOR_H_INCLUDED
