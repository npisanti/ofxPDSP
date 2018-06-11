
// DPWTri.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_DPWTRIANGLE_H_INCLUDED
#define PDSP_OSC_DPWTRIANGLE_H_INCLUDED

#include "../base/Oscillator.h"

namespace pdsp {
    /*!
    @brief Antialiased Triangle Oscillator
    
    This is an oscillator that generates a differential parabolic triangle waveform, a special kind of antialiased triangle. Anyway in most cases CheapTriangle is a valid alternative as triangle waveform have really few aliased harmonics.
    */

class DPWTri : public Oscillator{
public:
    DPWTri();

private:
    void prepareOscillator( double sampleRate) override;
    void releaseOscillator() override;
    void oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept override ;

    float samplerate;
    float z_1;
};

}


#endif  // PDSP_OSC_DPWTRIANGLE_H_INCLUDED
