
// BitNoise.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SOURCES_BITNOISE_H_INCLUDED
#define PDSP_SOURCES_BITNOISE_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/random/WhiteNoise.h"
#include "../../DSP/signal/Bitcruncher.h"
#include "../../DSP/signal/Decimator.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/utility/PitchToFreq.h"


namespace pdsp{
    
    /*!
    @brief Stereo Digital noise generator with lots of control.
    */   
    
class BitNoise : public Patchable {
    
public:
    BitNoise();
    BitNoise(const BitNoise &Other);
    BitNoise& operator=(const BitNoise &Other);

    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. This is the default input. This is the noise oscillator pitch. Default value is -100.0f (it sounds mostly non-pitched).
    */      
    Patchable& in_pitch();

    /*!
    @brief Sets "decimation" as selected input and returns this module ready to be patched. This is pitch frequency of the sample rate decimation. Default is 151 = 50175hz (when the frequency is higher than the sample rate there is no decimation). 
    */      
    Patchable& in_decimation();

    /*!
    @brief Sets "bits" as selected input and returns this module ready to be patched. The bits of the noise output, default is 12bits.
    */  
    Patchable& in_bits();
    
    /*!
    @brief Sets "trig" as selected input and returns this module ready to be patched. Connect a "trig" output to this input to activate retriggering of the noise seed and cycle.
    */  
    Patchable& in_trig();
    
    /*!
    @brief Uses the selected channel as output for the patching operation. 0 is for the left channel (default output) and 1 is for the right channel. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );

    /*!
    @brief Uses the selected channel as output for the patching operation. 0 is for the left channel (default output) and 1 is for the right channel. This is the noise output before passing in the decimator and bitcruncher. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch_noise( size_t index );
    

/*!
    @cond HIDDEN_SYMBOLS
*/    
    [[deprecated("out_0() deprecated, use ch(0) instead")]]   
    Patchable& out_0();
    
    [[deprecated("out_1() deprecated, use ch(0) instead")]]   
    Patchable& out_1();
    
    [[deprecated("out_noise() deprecated, use ch_noise(0) instead")]]   
    Patchable& out_noise();
    
    [[deprecated("out_decimated() deprecated, just patch instead, it is the default output")]]
    Patchable& out_decimated();
    
    [[deprecated("out_noise_0() deprecated, use ch_noise(0) instead")]] 
    Patchable& out_noise_0();
    
    [[deprecated("out_noise_1() deprecated, use ch_noise(1) instead")]]
    Patchable& out_noise_1();
    
    [[deprecated("out_L() deprecated, use ch(0) instead")]]   
    Patchable& out_L();
    
    [[deprecated("out_R() deprecated, use ch(1) instead")]]
    Patchable& out_R();

    [[deprecated("out_noise_L() deprecated, use ch_noise(0) instead")]]   
    Patchable& out_noise_L();
    
    [[deprecated("out_noise_R() deprecated, use ch_noise(1) instead")]]
    Patchable& out_noise_R();
/*!
    @endcond
*/
    
    
private:
    void patch();
        
    PatchNode       trigger;
    PatchNode       bits_ctrl;
    
    PitchToFreq     p2fPhazor;
    PitchToFreq     p2fDecimator;
    
    LFOPhasor       phasor;

    WhiteNoise      noiseA;
    Decimator       decimateA;
    Bitcruncher     bitcrunchA;

    WhiteNoise      noiseB;
    Decimator       decimateB;
    Bitcruncher     bitcrunchB;

};

} //END NAMESPACE


#endif // PDSP_SOURCES_BITNOISE_H_INCLUDED
