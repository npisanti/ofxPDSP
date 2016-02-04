
// ofxMidiDrumKitProcessor
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_MIDIDRUMKITPROCESSOR_H_INCLUDED
#define OFXPDSP_MIDIDRUMKITPROCESSOR_H_INCLUDED

#include "ofxMidi.h"
#include "../messages/header.h"
#include "../DSP/control/GateSequencer.h"
#include "ofxPositionedMidiMessage.h"

#include "ofxMidiInProcessor.h"

class ofxMidiDrumKitProcessor {

    
public:
    ofxMidiDrumKitProcessor();

    void processMidi(const ofxMidiInProcessor &midiInProcessor, const int &bufferSize ) noexcept;
    
    void addTriggerLayer(int noteLow, int noteHigh);
    void setTriggerLayer(int noteLow, int noteHigh, int outIndex);
    void resizeLayers(int size);
    
    void simpleInit(int lowNote, int numNotes, int span=1);

    vector<pdsp::GateSequencer>     outs_trig;

private:
    vector<pdsp::MessageBuffer>     trigBuffers;       
    vector<int>                     lowThreshold;
    vector<int>                     highThreshold;

    int size;
};







#endif // OFXPDSP_MIDIDRUMKITPROCESSOR_H_INCLUDED
