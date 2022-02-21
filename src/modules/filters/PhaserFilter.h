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
@brief A 4 pole Phaser. Multichannel.
* 
*/       

class PhaserFilter : public Patchable {

public:
    PhaserFilter();
    PhaserFilter(const PhaserFilter& other);
    PhaserFilter& operator=(const PhaserFilter& other);
    ~PhaserFilter();

    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the filter input.
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
    @brief Sets "signal" as selected output and returns this module ready to be patched. This is the default output. This is filter signal output.
    */  
    Patchable& out_signal();

    /*!
    @brief Allocate a number of channels for processing different inputs. This is automatically called if you query for a channel outside the allocated range. You can access different channels with the ch() method.
    @param[in] size number of channels
    */        
    void channels( std::size_t size );    

    /*!
    @brief Uses the selected channel as input/output for the patching operation.
    @param[in] index channel index
    */  
    Patchable& ch( std::size_t index );
    
    
    /*!
    @brief returns the actual pitindex value.This method is thread-safe.
    */  
    float meter_pitch() const;

/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("operator[] deprecated, use the ch( int index ) method instead")]]    
    Patchable& operator[]( std::size_t index );
/*!
    @endcond HIDDEN_SYMBOLS
*/    

private:
    
    void patch();


    std::vector<APF4*> phasers;
    
    PitchToFreq p2f;
    PatchNode   fbcontrol;
    PatchNode   spreadcontrol;


};


}

#endif //PDSP_MODULE_PHASERFILTER_H_INCLUDED
