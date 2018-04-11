
// ofxMidiCCBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"
#include "../messages/header.h"
#include "ofxPositionedMidiMessage.h"

class ofxMidiCCBuffers {
public:
    ofxMidiCCBuffers();

    void processMidi (const std::vector<_ofxPositionedMidiMessage> & readVector, const int &bufferSize );
    
    void clearAll();
    
    void setMaxCCNum(int maxCC);
    
    int getCCSize(){ return ccSize; }

    std::vector<pdsp::MessageBuffer>     ccMessages;



private:

    bool    sendClearMessages; 
    int     ccSize;
    
};

#endif

#endif // OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
