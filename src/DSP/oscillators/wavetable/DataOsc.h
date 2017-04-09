// DataOsc.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2017

#ifndef PDSP_OSC_DATAOSC_H_INCLUDED
#define PDSP_OSC_DATAOSC_H_INCLUDED

#include "../base/Oscillator.h"
#include "DataTable.h"

namespace pdsp{

    /*!
    @brief Data table oscillator
    
    This is a wavetable oscillator that use a pdsp::DataTable to trasform data to waveforme in realtime.
    */

class DataOsc : public Oscillator {
public:
    DataOsc();
    DataOsc(const DataOsc& other);
    DataOsc& operator=(const DataOsc& other);
    
    /*!
    @brief sets the DataTable. 
    @param[in] dataTable DataTable class object
    */
    void setTable(DataTable& dataTable);
    
private:
    void prepareOscillator(double sampleRate) override;
    
    void oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept override;
    
    DataTable* dataTable;
};

} // end pdsp namespace

#endif  // PDSP_OSC_WAVETABLEOSC_H_INCLUDED
