
// RMSDetector.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DYN_RMSDETECTOR_H_INCLUDED
#define PDSP_DYN_RMSDETECTOR_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
    /*!
    @brief An RMS signal detector.
    
    This Unit makes for each samples the Root Mean Squared of the values inside a given time window. It is useful as first step for dynamic processing. If set(float window_ms) is not used the window is to 50ms.

    */
    
class RMSDetector : public Unit{

public:        
    RMSDetector();

    /*!
    @brief sets the window millisecond value and returns this unit ready to be patched. An array of values will be deallocated and the allocated again so this is ABSOLUTELY not thread-safe to run in the audio-thread. 
    @param[in] window_ms value in milliseconds of the RMS-window time.
    */    
    Patchable& set(float window_ms);
 
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This is the detector input.
    */   
    Patchable& in_signal(); 

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the detector output.
    */ 
    Patchable& out_signal();
    
    /*!
    @brief return the first value of the last processed output buffer.
    */ 
    float meter_output() const;

    /*!
    @brief return the ms window in samples, useful for set a delay for lookahead correction with a SamplesDelay.
    */ 
    int getWindowSamples() const ;
    
protected:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

private:
    void updateWindowBuffer();
    void setWindowTime(float window_ms);  
        
    InputNode       input;
    OutputNode      output;

    float   window_ms;
    
    float   actualSum;
    int     readIndex;

    int     bufferLength;
    float*  squaresDividedBuffer;
    float   windowDivisor;
    
    float   sampleRateMultiplier;
    
    std::atomic<float> meter;
    
};

}//END NAMESPACE

#endif  // PDSP_DYN_RMSDETECTOR_H_INCLUDED
