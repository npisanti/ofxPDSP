
#ifndef OFXPDSP_PDSPCONTROLLER_H_INCLUDED
#define OFXPDSP_PDSPCONTROLLER_H_INCLUDED

#include "ofxPDSPMidiIn.h"

class ofxPDSPController{
public:

    virtual void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept = 0;
    
};


#endif // OFXPDSP_PDSPCONTROLLER_H_INCLUDED
