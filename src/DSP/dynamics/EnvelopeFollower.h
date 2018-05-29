
// EnvelopeFollower.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_DYN_ENVELOPEFOLLOWER_H_INCLUDED
#define PDSP_DYN_ENVELOPEFOLLOWER_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

    /*!
    @brief Envelope Follower for dynamic processing and control.
    
    EnvelopeFollower slews the input signal with a variable rise ( in_attack() ) and decay ( in_release() ) times. Usually it is used after some kind of processing to make an audio signal unipolar, like HalfWavePeakDetector or RMSDetector . It essential to implement dynamic processing modules.

    */

class EnvelopeFollower : public Unit{

public:

    EnvelopeFollower();
    
    /*!
    @brief sets the default attack and release values and return the unit ready to be patched.
    @param[in] attack_ms attack time in milliseconds. Default before this call is 20ms.
    @param[in] release_ms release time in milliseconds. Default before this call is 50ms.
    */
    Patchable& set(float attack_ms, float release_ms);
    
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal/value to slew.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "attack" as selected input and returns this Unit ready to be patched. This input is the rise ( aka attack ) time of the envelope follower.
    */   
    Patchable& in_attack();
    
    /*!
    @brief Sets "release" as selected input and returns this Unit ready to be patched. This input is the decay ( aka release ) time of the envelope follower.
    */  
    Patchable& in_release();
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is enveloped output.
    */ 
    Patchable& out_signal();
    
    /*!
    @brief returns the first output value of the last processed buffer.
    */ 
    float meter_output() const;    


    /*!
    @brief sets the envelope behavior digital (more responsive, default behavior).
    */     
    void digital();

    /*!
    @brief sets the envelope behavior to analog (slower).
    */     
    void analog();
    
        
private:

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    InputNode input;
    InputNode in_attack_ms;
    InputNode in_release_ms;
    OutputNode output;

    float attackCoeff;
    float releaseCoeff;
    float envelopeOutput;
    float TC;
    
    float sampleRate;
    
    std::atomic<float> meter;
};
        
        
}//END NAMESPACE

#endif  // PDSP_DYN_ENVELOPEFOLLOWER_H_INCLUDED
