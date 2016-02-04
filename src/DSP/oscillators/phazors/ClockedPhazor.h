
// ClockedPhazor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_PHAZOR_CLOCKEDPHAZOR_H_INCLUDED
#define PDSP_PHAZOR_CLOCKEDPHAZOR_H_INCLUDED


#include "../../pdspCore.h"


namespace pdsp {

    /*!
    @brief Phazor clocked to tempo values and syncronized to transport
    
    This phazor is clocked to the global tempo and has a division and phase-offset inpuit. It syncronize itself to the global tempo or it can be retriggered by a "trig" output
    */

class ClockedPhazor :   public Unit,
                        public Clockable
{

public:
    ClockedPhazor();

    /*!
    @brief sets the default time division and phase offset values and returns the unit ready to be patched.
    @param[in] division division time, for example 4.0f is one quarter/cycle and 0.25f means 4 bars/cycle
    @param[in] phaseOffset phase offset at retrigger or in relation to the given division
    */
    Patchable& set(float division, float phaseOffset);
   
    /*!
    @brief Sets "retrig" as selected input and returns this Unit ready to be patched. This is the default input. Connect a "trig" output to this input to activate manual retriggering of the phazor, otherwise the phasor is synced to the transport
    */  
    Patchable& in_retrig();

    /*!
    @brief Sets "phase_offset" as selected input and returns this Unit ready to be patched. This is the offset of the phazor at retrigger or in relation to the given division. It is updated only at control-rate.
    */
    Patchable& in_phase_offset();
    
    /*!
    @brief Sets "division" as selected input and returns this Unit ready to be patched. This is the time division, higher is faster, for example 16.0f means 1/16th and 0.5f means two bars/cycle. It is updated only at control-rate.
    */
    Patchable& in_division();
    
    /*!
    @brief Sets "phase" as selected output and returns this Unit ready to be patched. This is the default output. This is the phase output to be patched into an oscillator in_phase(). 
    */ 
    Patchable& out_phase();
    
    /*!
    @brief Sets "trig" as selected output and returns this Unit ready to be patched. This outputs sets a trigger on value each time the phazor restarts its cycle, it can be useful to trigger other units.
    */     
    Patchable& out_trig();
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override;
    void process (int bufferSize) noexcept override;
    
    void processTrigger(int bufferSize) noexcept;
    void processSync(int bufferSize) noexcept;
    void onReset(const int &n, const float &inc, float* &triggerBuffer, const int &bufferSize);
   
    InputNode   input_trig;

    InputNode   input_phase;
    InputNode   input_division;

    OutputNode  output_phase;
    OutputNode  output_trig;       
    
    float division;
    float phaseOffset;
    float phase;
    float oversampleFactor;

};
}



#endif  // PDSP_PHAZOR_CLOCKEDPHAZOR_H_INCLUDED
