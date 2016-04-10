
// ofxMidiCCBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED

#include "ofxMidi.h"
#include "../messages/header.h"
#include "ofxPositionedMidiMessage.h"

class ofxMidiCCBuffers {
public:
    ofxMidiCCBuffers();

    void processMidi (const vector<_ofxPositionedMidiMessage> & readVector, const int &bufferSize );
    
    void clearAll();
    
    void setMaxCCNum(int maxCC);
    
    int getCCSize(){ return ccSize; }

    vector<pdsp::MessageBuffer>     ccMessages;



private:

    bool    sendClearMessages; 
    int     ccSize;
    
};


#endif // OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
