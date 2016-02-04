
// SineFB.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_SINEFB_H_INCLUDED
#define PDSP_OSC_SINEFB_H_INCLUDED


#include "../base/OscillatorVariShape.h"


namespace pdsp{

    /*!
    @brief Wavetable sine oscillator with self-fm
    
    This is a sine oscillator implemented with a 4096 point linearly interpolated wavetable. It performs really good as sine oscillator and fm operator. It has self-FM, controlled with in_shape() and should go from 0.0f to 4.0f (in_shape() is not clamped, beware).
    */
    
class SineFB : public OscillatorVariShape
{

public:

    SineFB();
    ~SineFB();
    /*!
    @brief sets the default self-FM amount value and returns the unit ready to be patched.
    @param[in] fmFeedback self-FM value, input of this function is clamped to 0.0f-4.0f .
    */
    Patchable& set(float fmFeedback);
    
    
private:
    void prepareOscillator( double sampleRate);
    void releaseOscillator();

    void oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept override;
    void oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept override;

    float fb_path;
    float alpha;
    float z1;

    static float* sineTable;
    static const float tableFakeSize;
    static uint64_t sineOscillatorsCreated;

};

}





#endif  // PDSP_OSC_SINEFB_H_INCLUDED
