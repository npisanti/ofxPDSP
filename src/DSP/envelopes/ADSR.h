
// ADSR.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_ENV_ADSRLEG_H_INCLUDED
#define PDSP_ENV_ADSRLEG_H_INCLUDED

#include "../pdspCore.h"
#include "stages/EnvelopeStage.h"
#include "stages/AttackStage.h"
#include "stages/DecayStage.h"
#include "stages/RiseStage.h"
#include "stages/SustainStage.h"
#include "stages/ReleaseStage.h"

namespace pdsp{
 
    /*!
    @brief Attack-Decay-Sustain-Release envelope.
    
    This is a standard ADSR envelope. This envelope takes a trigger output as input and produces an envelope based on the set times. It takes trigger on, off and legato values.

    */ 
        
class ADSR :    public Unit,
                public virtual EnvelopeStage,
                public virtual AttackStage,
                public virtual DecayStage, 
                public virtual RiseStage,
                public virtual SustainStage,
                public virtual ReleaseStage 
{

public:
    ADSR();

    /*!
    @brief sets the default envelope values and returns the unit ready to be pached in a chain.
    @param[in] attackTimeMs attack time
    @param[in] decayTimeMs decay time
    @param[in] sustainLevel sustain level
    @param[in] releaseTimeMs release time
    */
    Patchable& set(float attackTimeMs, float decayTimeMs, float sustainLevel, float releaseTimeMs);

    /*!
    @brief Sets "trig" as selected input and returns this Unit ready to be patched. This is the default input. This input is the trigger input of the envelope, you should patch an out_trig() to this input.
    */   
    Patchable& in_trig();
    
    /*!
    @brief Sets "attack" as selected input and returns this Unit ready to be patched. This input sets the attack time in milliseconds. The value is taken from this input once every trigger. Init time is 0 milliseconds.
    */       
    Patchable& in_attack();
    
    /*!
    @brief Sets "decay" as selected input and returns this Unit ready to be patched. This input sets the decay time in milliseconds. The value is taken from this input once every trigger. Init time is 150 milliseconds.
    */       
    Patchable& in_decay();
    
    /*!
    @brief Sets "sustain" as selected input and returns this Unit ready to be patched. This input sets the sustain value of the envelope. Init value is 0.5f. 
    */           
    Patchable& in_sustain();
    
    /*!
    @brief Sets "release" as selected input and returns this Unit ready to be patched. This input sets the release time in milliseconds. The value is taken from this input once every trigger. Init time is 150 milliseconds.
    */       
    Patchable& in_release();
    
    /*!
    @brief Sets "velocity" as selected input and returns this Unit ready to be patched. This input rapresent the influcence of trigger values on the envelope out. It ranges from 0.0f to 1.0f, at 0.0 the envelope output level is indipendent from the input trigger values. Init value is 1.0f.
    */       
    Patchable& in_velocity();    

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the envelope output.
    */ 
    Patchable& out_signal();

    /*!
    @brief Sets "bias" as selected output and returns this Unit ready to be patched. This is the negation of the actual sustain level, so if you add this to the envelope output you have an envelope that sustains always at 0.0f .
    */     
    Patchable& out_bias();

    /*!
    @brief returns the first value of the last processed output buffer. This method is thread-safe.
    */    
    float meter_output() const;

private:

    void process(int bufferSize) noexcept;
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources() override;

    inline_f void doEnvelope() {
            switch (stageSwitch){
            case off         : envelopeOutput = 0.0; break;
            case attackStage : Attack(stageSwitch, decayStage); break;
            case decayStage  : Decay(stageSwitch, sustainStage); break;
            case sustainStage: Sustain(); break;
            case releaseStage: Release(stageSwitch, off); break;
            case riseStage   : Rise(stageSwitch, sustainStage); break;
            }
    };

    void onRetrigger(float triggerValue, int n);
    
    void process_run(int bufferSize);
    void process_T( const float* &trigBuffer, const int &bufferSize);
    
    InputNode input_trig;
    OutputNode output;
    InputNode input_attack;
    InputNode input_decay;
    InputNode input_sustain;
    InputNode input_release;
    InputNode input_velocity;

    
    
    int              stageSwitch;
    
    static const int off          = 0;
    static const int attackStage  = 1;
    static const int decayStage   = 2;
    static const int sustainStage = 3;
    static const int releaseStage = 4;
    static const int riseStage    = 5;
    
    std::atomic<float> meter;
};

} // pdsp namespace end

#endif  // PDSP_ENV_ADSRLEG_H_INCLUDED
