
// ofxPositionedMidiMessage
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED
#define OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"


/*!
    @cond HIDDEN_SYMBOLS
*/

namespace pdsp{

class _PositionedMidiMessage{
public:
    _PositionedMidiMessage(){ sample = -1; };
    _PositionedMidiMessage(ofxMidiMessage message, int sample) : message(message), sample(sample){};
    
    ofxMidiMessage message;
    int sample;
};

}

/*!
    @endcond
*/

#endif

#endif// OFXPDSPMIDI_POSITIONEDMIDIMESSAGE_H_INCLUDED
