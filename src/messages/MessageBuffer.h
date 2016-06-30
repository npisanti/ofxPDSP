
// MessageBuffer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED
#define PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED

#include "Message.h"
#include <vector>
#include "../DSP/control/Sequencer.h"
#include "../flags.h"
#include "ExtSequencer.h"

namespace pdsp{

    class Sequencer;
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    class MessageBuffer{
        friend void linkSelectedOutputToExtSequencer    (MessageBuffer& messageBuffer, ExtSequencer& ext);
        friend void unlinkSelectedOutputToExtSequencer  (MessageBuffer& messageBuffer, ExtSequencer& ext);

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
    

    void linkSelectedOutputToExtSequencer (MessageBuffer& messageBuffer, ExtSequencer& ext);
    void operator>> (MessageBuffer& messageBuffer, ExtSequencer& ext);

    void unlinkSelectedOutputToExtSequencer (MessageBuffer& messageBuffer, ExtSequencer& ext);
    void operator!= (MessageBuffer& messageBuffer, ExtSequencer& ext);    

/*!
    @endcond
*/

}//END NAMESPACE

#endif  // PDSP_MESSAGES_MESSAGEBUFFER_H_INCLUDED
