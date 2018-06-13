
#ifndef OFXPDSP_PDSPCONTROLLER_H_INCLUDED
#define OFXPDSP_PDSPCONTROLLER_H_INCLUDED

#ifndef __ANDROID__

#include "../MidiIn.h"

namespace pdsp{

class Controller{
public:

    virtual void processMidi(const pdsp::midi::Input &midiInProcessor, const int &bufferSize ) noexcept = 0;
    
};

}

#endif

#endif // OFXPDSP_PDSPCONTROLLER_H_INCLUDED
