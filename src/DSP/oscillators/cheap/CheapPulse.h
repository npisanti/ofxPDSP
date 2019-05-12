
// CheapPulse.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_OSC_CHEAPPULSE_H_INCLUDED
#define PDSP_OSC_CHEAPPULSE_H_INCLUDED


#include "../base/OscillatorVariShape.h"


namespace pdsp{
    /*!
    @brief Economic pulse oscillator
    
    This is a trivial pulse oscillator, in_shape() controls the pulse width but it is not clamped, keep the range between 0.0f and 1.0f
    */

class CheapPulse : public OscillatorVariShape {

public:
    CheapPulse();

    /*!
    @brief sets the default pulse width values and returns the unit ready to be patched.
    @param[in] pulseWidth pulse width value
    */
    Patchable& set(float pulseWidth);

private:
    void prepareOscillator( double sampleRate )override {};
    void releaseOscillator()override {};

    void oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept override;
    void oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept override;
};

} // end pdsp namespace


#endif  // PDSP_OSC_CheapPulse_H_INCLUDED
