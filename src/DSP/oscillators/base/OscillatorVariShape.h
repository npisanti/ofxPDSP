
// OscillatorVariShape.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_OSCILLATORVARISHAPE_H_INCLUDED
#define PDSP_OSC_OSCILLATORVARISHAPE_H_INCLUDED

#include "../../pdspCore.h"



namespace pdsp{
   /*!
    @brief Abstract class to implement oscillators with single shape control
    
    This is a abstract class to rapresent oscillators, with a phase input an a shape control input. You usually have just to implement the oscillateShapeCR() and oscillateShapeAR() methods
    */
class OscillatorVariShape : public Unit
{

public:

    OscillatorVariShape();
    

    
    /*!
    @brief Sets "phase" as selected input and returns this Unit ready to be patched. This is the default input. You have to patch a "phase" output to this input, usuall taken from some kind of phazor like PMPhazor, LFOPhazor or ClockedPhazor
    */     
    Patchable& in_phase();
    
    /*!
    @brief Sets "shape" as selected input and returns this Unit ready to be patched. This is the shape input, it supports audio-rate modulation.
    */       
    Patchable& in_shape();


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
    @brief method to implement for preparing oscillator to play, with control-rate shape given
    @param[out] outputBuffer buffer to fill with waveform output
    @param[in] phaseBuffer buffer for the phase values
    @param[in] shape shape control values
    @param[in] bufferSize buffersize, already multipled if oversampling 
    */     
    virtual void oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept =0;
  
    /*!
    @brief method to implement for preparing oscillator to play, with audio-rate shape given
    @param[out] outputBuffer buffer to fill with waveform output
    @param[in] phaseBuffer buffer for the phase values
    @param[in] shapeBuffer buffer of shape control values 
    @param[in] bufferSize buffersize, already multipled if oversampling 
    */       
    virtual void oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept =0;
    
    InputNode input_shape;
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources() override;
    void process(int bufferSize) noexcept override;


    InputNode input_phase;
    OutputNode output;
    
    std::atomic<float> meter;

};
    
}//END NAMESPACE




#endif  // PDSP_OSC_OSCILLATORVARISHAPE_H_INCLUDED
