
// CheapTri.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_OSC_CHEAPTRI_H_INCLUDED
#define PDSP_OSC_CHEAPTRI_H_INCLUDED

#include "../base/Oscillator.h"

namespace pdsp{

    /*!
    @brief Economic triangle oscillator
    
    This is a trivial triangle oscillator. As triangle is a waveform with few harmonics there isn't so much aliasing so this oscillator has a good sounds also when used at audio frequencies. If you want something better you should try DPWTriangle.
    */

class CheapTri : public Oscillator{
private:
        void oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept override;
};


} // end pdsp namespace


#endif  // PDSP_OSC_CheapTri_H_INCLUDED
