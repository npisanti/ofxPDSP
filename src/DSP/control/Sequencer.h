
// Sequencer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_SEQUENCER_H_INCLUDED
#define PDSP_CONTROL_SEQUENCER_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
class MessageBuffer;
        
class Sequencer : public Unit{
        friend Patchable& linkBufferToSequencer (MessageBuffer& output, Sequencer& input);
        friend MessageBuffer;

public:
        virtual void link(MessageBuffer &messageBuffer) = 0;
        virtual void unLink() = 0;
        
        //TODO: add operator for linking MessageSources to sequencer
protected:
        virtual void resetMessageBufferSelector() {};
        
};
        
Patchable& linkBufferToSequencer (MessageBuffer& output, Sequencer& input);
Patchable& operator>> (MessageBuffer& output, Sequencer& input);
        
        
        
} // pdsp namespace end



#endif  // PDSP_CONTROL_SEQUENCER_H_INCLUDED
