
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
    @param[in] velocity sensitivity to trigger values
    */
    Patchable& set(float attackTimeMs, float decayTimeMs, float sustainLevel, float releaseTimeMs, float velocity = 1.0f );


    /*!
    @brief sets the curve of the attack stage the envelope, from a smoother linear in dB curve to an harder analog-like curve. By default is 0.0f ( linear in dB attack ).
    @param[in] hardness how much analog-like the analog curve is, value is clampet to the 0.0<-->1.0f range, 
    */
    void setAttackCurve(float hardness); 
 
    /*!
    @brief sets the curve of the decay and release stages of the envelope, from a smoother linear in dB curve to an harder analog-like curve. By default is 1.0f ( analog-like releases ).
    @param[in] hardness how much analog-like the analog curve is, value is clampet to the 0.0<-->1.0f range, 
    */
    void setReleaseCurve(float hardness); 

    /*!
    @brief sets the curve of the envelope, from a smoother linear in dB curve to an harder analog-like curve.
    @param[in] hardness how much analog-like the analog curve is, value is clampet to the 0.0<-->1.0f range, 
    */
    void setCurve(float hardness);


    /*!
    @brief enable an alternate mode for the trigger dynamics, trigger will output linear value that are the equivalent of dBmin at 0.0f and dBmax at 1.0f
    @param[in] dBmin minimum dB output
    @param[in] dBmax maximum dB output, 0 dB if not given
    */
    void enableDBTriggering(float dBmin, float dBmax=0.0f);
    
    /*!
    @brief disable the alternate mode for the trigger dynamics.
    */
    void disableDBTriggering();

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

    void process(int bufferSize) noexcept override;
    void prepareUnit( int expectedBufferSize, double sampleRate) override;
    void releaseResources() override;

    inline_f void doEnvelope() {
            switch (stageSwitch){
            case off         : envelopeOutput = 0.0; break;
            case attackStage : Attack(stageSwitch, decayStage); break;
            case decayStage  : Decay(stageSwitch, sustainStage); break;
            case sustainStage: Sustain(); break;
            case releaseStage: Release(stageSwitch, off); break;
            case riseStage   : Rise(stageSwitch, decayStage); break;
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

    std::atomic<bool> dBtrig;
    float dBmin;
    float dBmax;    
    
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
