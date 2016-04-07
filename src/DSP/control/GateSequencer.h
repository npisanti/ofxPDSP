
// GateSequencer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_GATESEQUENCER_H_INCLUDED
#define PDSP_CONTROL_GATESEQUENCER_H_INCLUDED

#include "../pdspCore.h"
#include "Sequencer.h"

namespace pdsp{
        
/*!
@brief Takes messages from a MessageBuffer and convert them in a trigger output

Generate trigger values from a MessageBuffer. It also has a switch to implement "Single Trigger Mode". In single trigger mode just the first positive message will generate a standard trigger message, the other will generate some "legato" trigger signals that often will not retrigger the envelope from starts; a normal trigger will be emitted only after a message with 0.0f value is received. GateSequencer has no inputs, but you can use the >> operator to connect a MessageBuffer to it.
*/

class GateSequencer :  public Sequencer{
        
public:
        GateSequencer();
        GateSequencer(const GateSequencer& other);
        
        /*!
        @brief connect the GateSequencer to a MessageBuffer. You can also use the >> operator with the same effect.
        @param[in] messageBuffer MessageBuffer to connect
        */       
        void link(MessageBuffer &messageBuffer) override;
        
        /*!
        @brief disconnect the GateSequencer from the connected MessageBuffer.
        */           
        void unLink() override;
        
        /*!
        @brief Sets the behavior in the trigger signals generation. If true some of the connected envelopes will not restart from the first stage until a message with 0.0f is received (that will also set the stage to release). 
        @param[in] state boolean state, true for activate legato. Standard is false.
        */         
        void setSingleTrigger(bool state);
        
        /*!
        @brief Sets "trig" as selected output and returns this Unit ready to be patched. This is the default output. This is the trigger output to connect to envelopes or "trig" inputs.
        */   
        Patchable& out_trig();
        
        /*!
        @brief returns how many time this Unit has been processed since the last trigger, useful for visual. Thread-safe.
        */ 
        int meter_last_ticks() const;

        /*!
        @brief returns the last trigger value received. Thread-safe.
        */ 
        float meter_last_value() const;

private:

        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override;
        void process (int bufferSize) noexcept override;

        OutputNode output_trig;
        

        bool singleTrigger;
        bool gateState;
        
        std::atomic<int> meter_ticks;
        std::atomic<float> meter_value;
        
};
        
        
} // pdsp namespace end



#endif  // PDSP_CONTROL_GATESEQUENCER_H_INCLUDED
