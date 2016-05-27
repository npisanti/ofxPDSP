
// TriggerControl.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_TRIGGERCONTROL_H_INCLUDED
#define PDSP_CONTROL_TRIGGERCONTROL_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
/*!
@brief Generate trigger on/off messages when the input values are greater than a set value.

TriggerControl generate trigger on/off values when the trigger() or off() methods are called. Those methods are thread-safe.
*/

class TriggerControl : public Unit {

public:
        TriggerControl();
        
        TriggerControl(const TriggerControl & other);

        /*!
        @brief Sets "trig" as selected output and returns this Unit ready to be patched. This is the default output. This is the trigger output to connect to envelopes or "trig" inputs.
        */   
        Patchable& out_trig();

        /*!
        @brief A trigger with the given value will be emitted as soon as possible (usually this means at the start of the next audio buffer). A negative value emits a "legato" trigger, and you can use pdsp::off as value for generating a trigger off value. This method is thread safe.
        */    
        void trigger(float value);

        /*!
        @brief A trigger off will be emitted as soon as possible (usually this means at the start of the next audio buffer). This method is thread safe.
        */    
        void off();
        
private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        void process (int bufferSize) noexcept override;

        OutputNode  output;   
        
        std::atomic<float> nextTrigger;
        
};    

} // pdsp namespace end


#endif //PDSP_CONTROL_TRIGGERCONTROL_H_INCLUDED
