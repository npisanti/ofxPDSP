
#ifndef OFXPDSP_PDSPCONTROLLER_H_INCLUDED
#define OFXPDSP_PDSPCONTROLLER_H_INCLUDED

#ifndef __ANDROID__

#include "ofxPDSPMidiIn.h"

class ofxPDSPController{
public:

    virtual void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept = 0;
    
};

#endif

#endif // OFXPDSP_PDSPCONTROLLER_H_INCLUDED
