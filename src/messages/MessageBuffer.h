
// MessageBuffer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED
#define PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED

#include "Message.h"
#include <vector>
#include "../DSP/control/Sequencer.h"
#include "../flags.h"

namespace pdsp{

    class Sequencer;
/*!
    @cond HIDDEN_SYMBOLS
*/
    class MessageBuffer{

    public:
        MessageBuffer();
        MessageBuffer(const MessageBuffer & other);
        MessageBuffer& operator=(const MessageBuffer & other);
        
        void clearMessages();
        void addMessage(float value, int sample);
        void processDestination(const int &bufferSize);
        
        int  size();
        void reserve(int size); 
        bool empty();

        Sequencer*                      destination;
        std::vector<ControlMessage>     messages;
        bool                            connectedToGate;
            
    };
    
/*!
    @endcond
*/

}//END NAMESPACE

#endif  // PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED
