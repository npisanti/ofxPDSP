
// ExtSequencer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED
#define PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED

namespace pdsp{

class MessageBuffer;

class ExtSequencer{
    
public:
    virtual void linkToMessageBuffer(MessageBuffer &messageBuffer) = 0;
    virtual void unlinkMessageBuffer(MessageBuffer &messageBuffer) = 0;
};
    
}
#endif // PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED
