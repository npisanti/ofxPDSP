
// BLEPSaw.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_OSC_BLEPSAW_H_INCLUDED
#define PDSP_OSC_BLEPSAW_H_INCLUDED

#include "BLEP/BLEPBased.h"
#include "../base/Oscillator.h"


namespace pdsp{
    /*!
    @brief Antialiased Saw Oscillator
    
    This is an oscillator that generates a band-limited saw waveform, antialiasing is performed using BLEP tables, with the constructor you can select the window type, if 2 or 8 points are used, the lenght of the BLEP table and if the values are interpolated when performing BLEP. The standar constructor use a 2-point 4096-values table with Rectangular window (Rectangular == not windowed), with interpolation active.
    */
class BLEPSaw : public BLEPBased,
    public Oscillator {
            
public:
    BLEPSaw(Window_t window, bool eight, int length, bool interpolate);
    BLEPSaw();
    
    /*!
    @brief Sets "inc" as selected input and returns this Unit ready to be patched. It is mandatory to patch an "inc" output to this input for the BLEP algorithm to function. Usually PMPhazor have the required out_inc().
    */   
    Patchable& in_inc();
        
private:
    void prepareOscillator( double sampleRate) override;
    void releaseOscillator() override;
    inline void oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept override ;
    
    InputNode input_inc;

};

}





#endif  // PDSP_OSC_BLEPSAW_H_INCLUDED
