// PhaserFilter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2018

#ifndef PDSP_MODULE_PHASERFILTER_H_INCLUDED
#define PDSP_MODULE_PHASERFILTER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/filters/APF4.h"
#include "../../DSP/utility/PitchToFreq.h"


namespace pdsp{
/*!
@brief A 4pole Phaser, multichannel.
* 
*/       

class PhaserFilter : public Patchable {

public:
    PhaserFilter();
    PhaserFilter(const PhaserFilter& other);
    PhaserFilter& operator=(const PhaserFilter& other);
    ~PhaserFilter();

    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the filter input.
    */      
    Patchable& in_signal();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the tuning of the phaser in semitones.
    */  
    Patchable& in_pitch();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the tuning of the phaser in semitones.
    */  
    Patchable& in_cutoff();

    /*!
    @brief Sets "feedback" as selected input and returns this Unit ready to be patched. This is the phaser feedback amount, also try negative numbers.
    */
    Patchable& in_feedback();
    
    /*
    @brief Sets "spread" as selected input and returns this module ready to be patched. The value of the frequency of each pole is equal to the frequency of the pole before it plus spread multiplied by the base frequency. So at 0.0f all the pole have the same frequency, at 1.0f the poles form an harmonic series, etc etc
    */    
    Patchable& in_spread();

    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is filter signal output.
    */  
    Patchable& out_signal();

    /*!
    @brief sets the number of different inputs this module can process. Be sure to set the channels before doing any patching. You can access channels with in/out methods, using a string with their number as input, for example in("1") or out("4"), or by using ini(int ch) or outi(int ch) methods.
    @param[in] size number of channels
    */        
    void channels( int size );    

    /*!
    @brief To use the selected channel as input/output for the operation.
    @param[in] ch channel index
    */  
    Patchable& operator[]( const int & ch );
    
    
    /*!
    @brief returns the actual pitch value.This method is thread-safe.
    */  
    float meter_pitch() const;

private:
    
    void patch();


    std::vector<APF4*> phasers;
    
    PitchToFreq p2f;
    PatchNode   fbcontrol;
    PatchNode   spreadcontrol;


};


}

#endif //PDSP_MODULE_PHASERFILTER_H_INCLUDED
