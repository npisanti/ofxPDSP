
// SamplesDelay.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTILITY_SAMPLESDELAY_H_INCLUDED
#define PDSP_UTILITY_SAMPLESDELAY_H_INCLUDED

#include "../pdspCore.h"
#include "../interpolators/interpolators.h"

namespace pdsp{

    /*!
    @brief Utility to delay signal for a certain number of samples
    
    */
class SamplesDelay : public Unit {
public:

    SamplesDelay();

    SamplesDelay( int samples );


    /*!
    @brief sets the default delay time, returns the unit ready to be patched. Delay buffer will be deallocated and reallocated so absolutely NOT THREAD SAFE to run in the audio thread.
    @param[in] samples how many samples the signal will be delayed

    */     
    Patchable& set( int samples);
    
    /*!
    @brief sets the default delay time. Delay buffer will be deallocated and reallocated so absolutely NOT THREAD SAFE to run in the audio thread.
    @param[in] samples how many samples the signal will be delayed

    */     
    void setSamples( int samples);
    
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the signal to be delayed. You can patch the output back to this input to make a feedback path.
    */   
    Patchable& in_signal();
    
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the delayed signal.
    */       
    Patchable& out_signal();



private:
    void process(int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void initDelayBuffer();
    
    InputNode       input;
    
    OutputNode      output;
        
    
    float* delayBuffer;
    int maxDelayTimeSamples;
    

    int index;
    
    int silenceSamples;

};

} // pdsp namespace ned


#endif  // PDSP_UTILITY_SAMPLESDELAY_H_INCLUDED
