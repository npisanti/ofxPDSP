
// CheapSine.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_OSC_CHEAPSINE_H_INCLUDED
#define PDSP_OSC_CHEAPSINE_H_INCLUDED

#include "../base/Oscillator.h"


namespace pdsp{

    /*!
    @brief Economic sine oscillator
    
    This is parabolic approssimation of a sine waveform. In most of the cases SineFB (a wavetable sine oscillator) is a valid alternative, just check what oscillator fits your needs better.
    */


//parabolic approssimation of a sinusoidal waveform, with high and low precision
class CheapSine : public Oscillator{

private:
    void oscillate(float* outputBuffer, const float* phaseBuffer,int bufferSize) noexcept;
    
    static const float cheapSineB;
    static const float cheapSineC;
    static const float cheapSineP;

};//end CheapSineT class declaration



}//END NAMESPACE

#endif  // PDSP_OSC_CHEAPSINE_H_INCLUDED
