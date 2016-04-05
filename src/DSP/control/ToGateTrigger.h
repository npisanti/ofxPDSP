
// ToGateTrigger.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_TOGATETRIGGER_H_INCLUDED
#define PDSP_CONTROL_TOGATETRIGGER_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
/*!
@brief Generate trigger on/off messages when the input values are greater than a set value.

ToGateTrigger generate trigger on/off values from the values got from in_signal(). It also has an "hysteresis" parameter that is how much lower from the threshold the signal has to go to generate a trigger off value. If you want to use it with audio signal maybe it's a good idea to make them pass into another unit to make them positive and then got an envelope from them, like FullWavePeakDetector >> EnvelopeFollower . You can also set the threshold in realtime using in_threshold().
*/

class ToGateTrigger : public Unit {

public:
        ToGateTrigger();

        /*!
        @brief Sets the default threshold, optionally set the hysteresis and returns the Unit ready to be patched
        @param[in] threshold Default threshold value to set, it will be overrided by patching.
        @param[in] hysteresis Hysteresis value to set, if not specified is 0.0f .
        */
        Patchable& set(float threshold, float hysteresis = 0.0f);
      
        /*!
        @brief Sets the hysteresis.
        @param[in] hysteresis Hysteresis value to set.
        */
        void setHystersis(float hysteresis);
        
        /*!
        @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal that will be used to generate triggers.
        */ 
        Patchable& in_signal();
        
        /*!
        @brief Sets "threshold" as selected input and returns this Unit ready to be patched. This input is the threshold value to superate to activate the gate and emit trigger values. Default value is 0.5 . 
        */   
        Patchable& in_threshold();

        /*!
        @brief Sets "trig" as selected output and returns this Unit ready to be patched. This is the default output. This is the trigger output to connect to envelopes or "trig" inputs.
        */   
        Patchable& out_trig();
        
        /*!
        @brief returns the state of the gate, 1.0f for on / 0.0f for off . This method is thread safe.
        */    
        float meter_gate() const;

private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        void process (int bufferSize) noexcept override;

        void updateThresholds(float threshold);

        OutputNode  output;
        InputNode   input;
        
        InputNode   in_thresh;
        
        float   hysteresis;
        
        int     gate;        
        float   highThreshold;
        float   lowThreshold;
        
        float   thresholdMultiply;
        
        std::atomic<int> gateMeter;
};    

} // pdsp namespace end


#endif //PDSP_CONTROL_TOGATETRIGGER_H_INCLUDED
