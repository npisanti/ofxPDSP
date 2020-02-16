
// ExtSequencer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED
#define PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED

#include <vector>

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/ 
class MessageBuffer;
class Engine;

class ExtSequencer{
    friend class Engine;
    
public:
    ExtSequencer();
    ~ExtSequencer();

    virtual void linkToMessageBuffer(MessageBuffer &messageBuffer) = 0;
    virtual void unlinkMessageBuffer(MessageBuffer &messageBuffer) = 0;
    
    virtual void process( int bufferSize ) = 0;
    virtual void close() = 0;

private:
    static std::vector<ExtSequencer*> instances;
};
/*!
    @endcond
*/ 
}
#endif // PDSP_MESSAGES_EXTSEQUENCER_H_INCLUDED
