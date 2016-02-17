

#ifndef PDSP_OSC_WAVETABLEOSC_H_INCLUDED
#define PDSP_OSC_WAVETABLEOSC_H_INCLUDED

#include "../base/OscillatorVariShape.h"
#include "../../samplers/SampleBuffer.h"

namespace pdsp{

    /*!
    @brief Wavetable oscillator
    
    This is a wavetable oscillator that takes a pointer to a SampleBuffer and use that buffer as waveform table, where each waveform is a channel.
    */

class WaveTableOsc : public OscillatorVariShape {
public:
    WaveTableOsc();
    
    /*!
    @brief sets the Wavetable. The last channels of the wavetable is used as guard point for interpolation, so remember leaving it empty and to 0.0f
    @param[in] waveTable valid pointer to a SampleBuffer with a loaded waveform inside
    */
    void setTable(SampleBuffer* waveTable);
    
private:
    
    void oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept override;
    void oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept override;
    
    SampleBuffer* waveTable;
};

} // end pdsp namespace






#endif  // PDSP_OSC_WAVETABLEOSC_H_INCLUDED
