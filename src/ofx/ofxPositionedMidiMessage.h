
// ofxPositionedMidiMessage
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED
#define OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED

#include "ofxMidi.h"

class _ofxPositionedMidiMessage{
public:
    _ofxPositionedMidiMessage(){ sample = -1; };
    _ofxPositionedMidiMessage(ofxMidiMessage message, int sample) : message(message), sample(sample){};
    
    ofxMidiMessage message;
    int sample;
};


#endif// OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED
