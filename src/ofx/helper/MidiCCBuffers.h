
// MidiCCBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"
#include "../messages/header.h"
#include "PositionedMidiMessage.h"

namespace pdsp{ namespace helper {

class MidiCCBuffers {
public:
    MidiCCBuffers();

    void processMidi (const std::vector<_PositionedMidiMessage> & readVector, const int &bufferSize );
    
    void clearAll();
    
    void setMaxCCNum(int maxCC);
    
    int getCCSize(){ return ccSize; }

    std::vector<pdsp::MessageBuffer>     ccMessages;



private:

    bool    sendClearMessages; 
    int     ccSize;
    
};

}}

#endif

#endif // OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
