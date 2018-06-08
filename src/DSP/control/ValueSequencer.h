
// ValueSequencer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CONTROL_VALUESEQUENCER_H_INCLUDED
#define PDSP_CONTROL_VALUESEQUENCER_H_INCLUDED

#include "../pdspCore.h"
#include "Sequencer.h"
#include "../helpers/UsesSlew.h"


namespace pdsp{
/*!

@brief Takes messages from a MessageBuffer and convert them in an optionally smoothed value output

Generate smoothed values buffers from a MessageBuffer. You can set the slew time and also control it by patching a MessageBuffer to in_slew_control(). Patch this sequencer to pitch and other control inputs, while using GateSequencer to trigger envelopes. ValueSequencer has no inputs, but you can use the >> operator to connect a MessageBuffer to it.
*/

class ValueSequencer : public Sequencer, public UsesSlew {
    
public:
        ValueSequencer();
        ValueSequencer(const ValueSequencer & other);
                
        /*!
        @brief connect the ValueSequencer to a MessageBuffer. You can also use the >> operator with the same effect.
        @param[in] messageBuffer MessageBuffer to connect
        */             
        void link(MessageBuffer &messageBuffer) override;
       
       
        /*!
        @brief disconnect the ValueSequencer from the connected MessageBuffer.
        */        
        void unLink() override;
        
        /*!
        @brief Sets the standard message input as selected input, and returns this Sequence ready to be linked to a MessageSequencer with the >> operator. You are not patching input and output nodes this way but it works the same. The messages received in the input will be converted to a buffer of value ready to be patched into other Unit . Those values will be also smoothed by a settable slew time.
        */ 
        Sequencer& in_message();
        
        /*!
        @brief Sets an alternative message input as selected input, and returns this Sequence ready to be linked to a MessageSequencer with the >> operator. The value received from the message buffer will be used as a multiplier for set the slew time: You are not patching input and output nodes this way but it works the same.
        */ 
        Sequencer& in_slew();    
        
        /*!
        @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the slewed output.
        */ 
        Patchable& out_signal();

        /*!
        @brief returns the actual output value. Thread-safe.
        */ 
        float meter_output() const;


        /*!
        @brief enables the smoothing the outputted values
        @param[in] timeMs how many milliseconds will take to reach the setted value
        */  
        void enableSmoothing(float timeMs);
        
        /*!
        @brief disable the smoothing the outputted values. smoothing is disabled by default
        */  
        void disableSmoothing();
        
        /*!
        @brief don't smooth the next value received, like it was the first message.
        */  
        void resetSmoothing();

private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override;
        void process (int bufferSize) noexcept override;
        void resetMessageBufferSelector() override;
        
        OutputNode output;
        
        //MessageBuffer* messageBuffer;
        MessageBuffer* slewControl;
        bool connectToSlewControl;
        std::atomic<bool> firstMessage;
};

} // pdsp namespace end

#endif  // PDSP_CONTROL_VALUESEQUENCER_H_INCLUDED
