

#ifndef PDSP_OSC_WAVETABLEOSC_H_INCLUDED
#define PDSP_OSC_WAVETABLEOSC_H_INCLUDED

#include "../base/OscillatorVariShape.h"
#include "../../samplers/SampleBuffer.h"
#include "WaveTable.h"

namespace pdsp{

    /*!
    @brief Wavetable oscillator
    
    This is a wavetable oscillator that takes a pointer to a SampleBuffer and use that buffer as waveform table, where each waveform is a channel.
    */

class WaveTableOsc : public OscillatorVariShape {
public:
    WaveTableOsc();
    WaveTableOsc(const WaveTableOsc& other);
    WaveTableOsc& operator=(const WaveTableOsc& other);
    /*!
    @brief sets the Wavetable. 
    @param[in] waveTable WaveTable class object
    */
    void setTable(WaveTable& waveTable);

    /*!
    @brief returns the actual index of the wavetable, updated at control rate. Thread-safe.
    */    
    float meter_index() const;
private:
    
    void prepareOscillator(double sampleRate) override;
    
    void oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept override;
    void oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept override;
    
    WaveTable* waveTable;
    std::atomic<float> meter;
};

} // end pdsp namespace






#endif  // PDSP_OSC_WAVETABLEOSC_H_INCLUDED
