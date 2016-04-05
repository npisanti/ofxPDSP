
// ofxPDSPMidiControls.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED

#include "ofxMidi.h"
#include "../DSP/control/ValueSequencer.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include "ofxMidiCCBuffers.h"
#include "ofxPDSPController.h"
#include "ofxPDSPMidiIn.h"

class ofxPDSPMidiControls : public ofxPDSPController {
    
public:
    ofxPDSPMidiControls();
    ofxPDSPMidiControls(int maxCC);

    void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept override;
    	
    void setCCSlew(float slewTimeMs);
    void setMaxCCNum(int ccNum);
    int  getMaxCCNum();

    vector<pdsp::ValueSequencer>    outs_cc;

private: 
    float               slewTime;
    ofxMidiCCBuffers    midiCC;
        
};


#endif //OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED
