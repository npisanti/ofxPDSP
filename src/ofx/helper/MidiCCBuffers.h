
// MidiCCBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"
#include "../messages/header.h"
#include <map>
#include "PositionedMidiMessage.h"

namespace pdsp{ namespace helper {

class MidiCCBuffers {
public:
    MidiCCBuffers();

    void processMidi (const std::vector<_PositionedMidiMessage> & readVector, const int &bufferSize );

    void clearAll();

    pdsp::MessageBuffer & out(int cc);

    int getCCSize();

    std::map<int, pdsp::MessageBuffer> ccMessages;

    /*!
        @cond HIDDEN_SYMBOLS
    */
    void setMaxCCNum(int maxCC);
    /*!
        @endcond
    */

private:
    bool sendClearMessages;

};

}}

#endif

#endif // OFXPDSPMIDI_MIDICCBUFFERS_H_INCLUDED
