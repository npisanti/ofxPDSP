
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
    @brief Digital noise generator with lots of control.
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
    @brief Sets "decimated_0" as selected output and returns this module ready to be patched. This is the default output. This is the first channel of decimated and bitcrunched noise output.
    */     
    Patchable& out_decimated();

    /*!
    @brief Sets "noise_0" as selected output and returns this module ready to be patched. This is the first channel of noise output before passing in the decimator and bitcruncher. If you don't need those processors use this output for sparing cpu cycles.
    */     
    Patchable& out_noise();
    
    /*!
    @brief Sets "decimated_0" as selected output and returns this module ready to be patched. This is the default output. This is the first channel of decimated and bitcrunched noise output.
    */     
    Patchable& out_0();
    
    /*!
    @brief Sets "decimated_1" as selected output and returns this module ready to be patched. This is the second channel of decimated and bitcrunched noise output.
    */     
    Patchable& out_1();

    /*!
    @brief Sets "noise_0" as selected output and returns this module ready to be patched. This is the first channel of noise output before passing in the decimator and bitcruncher. If you don't need those processors use this output for sparing cpu cycles.
    */     
    Patchable& out_noise_0();
    
    /*!
    @brief Sets "noise_1" as selected output and returns this module ready to be patched. This is the second channel of noise output before passing in the decimator and bitcruncher. If you don't need those processors use this output for sparing cpu cycles.
    */     
    Patchable& out_noise_1();
    
    /*!
    @brief Sets "decimated_0" as selected output and returns this module ready to be patched. This is the default output. This is the first channel of decimated and bitcrunched noise output.
    */     
    Patchable& out_L();
    
    /*!
    @brief Sets "decimated_1" as selected output and returns this module ready to be patched. This is the second channel of decimated and bitcrunched noise output.
    */     
    Patchable& out_R();

    /*!
    @brief Sets "noise_0" as selected output and returns this module ready to be patched. This is the first channel of noise output before passing in the decimator and bitcruncher. If you don't need those processors use this output for sparing cpu cycles.
    */     
    Patchable& out_noise_L();
    
    /*!
    @brief Sets "noise_1" as selected output and returns this module ready to be patched. This is the second channel of noise output before passing in the decimator and bitcruncher. If you don't need those processors use this output for sparing cpu cycles.
    */     
    Patchable& out_noise_R();
    
    
    
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
