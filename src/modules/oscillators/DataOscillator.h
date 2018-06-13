
// DataOscillator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2017

#ifndef PDSP_MODULE_DATAOSCILLATOR_H_INCLUDED
#define PDSP_MODULE_DATAOSCILLATOR_H_INCLUDED

#include "../../DSP/pdspCore.h"

#include "../../DSP/oscillators/wavetable/DataOsc.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/utility/PitchToFreq.h"

namespace pdsp{

    /*!
    @brief DataTable based oscillator. Check out the pdsp::DataTable for more info
    */  

class DataOscillator : public Patchable {
    
public:

    DataOscillator();
    DataOscillator(const DataOscillator &Other);
    DataOscillator& operator=(const DataOscillator &Other);

    /*!
    @brief sets the Wavetable. 
    @param[in] dataTable DataTable for this oscillator
    */
    void setTable(DataTable& dataTable);

    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the default input. This is the operator pitch input.
    */      
    Patchable& in_pitch();
    
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

    DataOsc         wto;
    PitchToFreq     p2f;
    LFOPhasor       phasor;
    
};

} //END NAMESPACE


#endif // PDSP_MODULE_TABLEOSCILLATOR_H_INCLUDED
