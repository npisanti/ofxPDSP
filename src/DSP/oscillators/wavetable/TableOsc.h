

#ifndef PDSP_OSC_WAVETABLEOSC_H_INCLUDED
#define PDSP_OSC_WAVETABLEOSC_H_INCLUDED

#include "../base/Oscillator.h"
#include "../../samplers/SampleBuffer.h"

namespace pdsp{

    /*!
    @brief Wavetable oscillator
    
    This is a wavetable oscillator that takes a pointer to a SampleBuffer and use that buffer as waveform.
    */

class TableOsc : public Oscillator {
public:
    TableOsc();
    
    /*!
    @brief Wavetable oscillator
    @param[in] waveTable valid pointer to a SampleBuffer with a loaded waveform inside
    */
    void setTable(SampleBuffer* waveTable);
    
private:
    void prepareOscillator( double sampleRate ) override;
    void releaseOscillator() override;
    void oscillate( float* outputBuffer, const float* phaseBuffer, int sizeToProcess, int bufferSize) noexcept;
    
    SampleBuffer* waveTable;
};

} // end pdsp namespace






#endif  // PDSP_OSC_WAVETABLEOSC_H_INCLUDED
