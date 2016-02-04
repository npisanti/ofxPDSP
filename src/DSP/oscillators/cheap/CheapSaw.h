
// CheapSaw.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_CHEAPSAW_H_INCLUDED
#define PDSP_OSC_CHEAPSAW_H_INCLUDED

#include "../base/Oscillator.h"

namespace pdsp{
    /*!
    @brief Economic saw oscillator
    
    This is a trivial saw oscillator. It is full of aliased harmonics but easy on the cpu, good for things like LFOs. For a better saw oscillator use BLEPSaw
    */

class CheapSaw : public Oscillator{

private:
        void oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept override;

};


}


#endif  // PDSP_OSC_CheapSaw_H_INCLUDED
