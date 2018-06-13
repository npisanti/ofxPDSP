
// TableOscillator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_TABLEOSCILLATOR_H_INCLUDED
#define PDSP_MODULE_TABLEOSCILLATOR_H_INCLUDED

#include "../../DSP/pdspCore.h"

#include "../../DSP/oscillators/wavetable/WaveTableOsc.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/utility/PitchToFreq.h"



namespace pdsp{

    /*!
    @brief pdsp::WaveTable based oscillator
    */  

class TableOscillator : public Patchable {
    
public:

    TableOscillator();
    TableOscillator(const TableOscillator &Other);
    TableOscillator& operator=(const TableOscillator &Other);

    /*!
    @brief sets the Wavetable. 
    @param[in] waveTable WaveTable class object
    */
    void setTable(WaveTable& waveTable);

    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the default input. This is the operator pitch input.
    */      
    Patchable& in_pitch();

    /*!
    @brief Sets "table" as selected input and returns this module ready to be patched. This is the index of the wavetable.
    */             
    Patchable& in_table();

    
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the operator waveform output.
    */ 
    Patchable& out_signal();   


    /*!
    @brief meters the oscillator pitch. This method is thread-safe.
    */ 
    float meter_pitch() const;
    
private:
    void patch();

    WaveTableOsc    wto;
    PitchToFreq     p2f;
    LFOPhasor       phasor;
    
};

} //END NAMESPACE


#endif // PDSP_MODULE_TABLEOSCILLATOR_H_INCLUDED
