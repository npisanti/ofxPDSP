
// ofxMidiCCProcessor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDICCPROCESSOR_H_INCLUDED
#define OFXPDSPMIDI_MIDICCPROCESSOR_H_INCLUDED


#include "ofxMidi.h"
#include "../DSP/control/ValueSequencer.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include "ofxMidiCCBuffers.h"

#include "ofxMidiInProcessor.h"

class ofxMidiCCProcessor {
    
public:
    ofxMidiCCProcessor();
    ofxMidiCCProcessor(int maxCC);

    void processMidi(const ofxMidiInProcessor &midiInProcessor, const int &bufferSize ) noexcept;
    	
    void setCCSlew(float slewTimeMs);
    void setMaxCCNum(int ccNum);
    int  getMaxCCNum();

    vector<pdsp::ValueSequencer>    outs_cc;

private: 
    float               slewTime;
    ofxMidiCCBuffers    midiCC;
        
};


#endif //OFXPDSPMIDI_MIDICCPROCESSOR_H_INCLUDED
