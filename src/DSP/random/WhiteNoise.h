
// WhiteNoise.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_RANDOM_PRNOISEGEN_H_INCLUDED
#define PDSP_RANDOM_PRNOISEGEN_H_INCLUDED

#include <time.h>
#include "../pdspCore.h"


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : C4838)
#endif

#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wparentheses"
#endif


#define EXTRACT_BITS(the_val , bits_start , bits_len) ((the_val >> (bits_start-1)) & ((1<<bits_len-1)))

namespace pdsp{

static const float PNDivisor = 1.0 / 268435456.0; // 1/(2^32 / 16 )


inline_f float PNSequenceNext(uint32_t& PNRegister){
    //get the bits
    uint32_t b0  = EXTRACT_BITS(PNRegister, 1, 1); //1 = b0
    uint32_t b1  = EXTRACT_BITS(PNRegister, 2, 1); //2 = b1
    uint32_t b27 = EXTRACT_BITS(PNRegister, 28, 1); //28 = b27
    uint32_t b28 = EXTRACT_BITS(PNRegister, 29, 1); //29 = b28

    //form the XOR
    uint32_t b31 = b0^b1^b27^b28;

    //form the mask to OR with the register to load b31 
    if (b31 == 1){
            b31 = 0x10000000;
    }

    PNRegister >>= 1;
    PNRegister |= b31;

    return ((PNRegister * PNDivisor) - 1.0);

}

    /*!
    @brief PseudoRandom Noise Generator (used for white noise, but it can also generate more complex noise timbres, see pdsp::BitNoise)
    
    This is a pseudorandom noise generator with an input for changing the seed and another input for resetting the noise generator to the stored seed. If nothing is patched to this input it simply outputs white noise.
    */

class WhiteNoise : public Unit{
public:
    WhiteNoise();

    /*!
    @brief Sets "clock" as selected input and returns this Unit ready to be patched. This is the default input. You need to patch an out_trig() to this input. Each time there is a trigger on value the noise generator is resetted to the stored seed. It's almost like "tuning" the noise. Try with a LFOPhazor out_trig() output.
    */   
    Patchable& in_clock();
    
    /*!
    @brief Sets "reseed" as selected input and returns this Unit ready to be patched.  You need to patch an out_trig() to this input. Each time there is a trigger on value the noise generator stored seed is changed. Usually to ear the effect of this you should already be sending values to in_clock().
    */   
    Patchable& in_reseed();
    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the noise generator signal output.
    */ 
    Patchable& out_signal();
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;

    void process(int bufferSize) noexcept override;
    void process_run(const int &bufferSize) noexcept;
    void process_PA(const float* &trigSyncBuffer, const int &bufferSize) noexcept;
    void process_SA_TA(const float* &trigSyncBuffer, const float* &trigSeedBuffer, const int &bufferSize) noexcept;
            
    inline_f void reSeed();
    
    OutputNode output;
    InputNode input_trig_clock;
    InputNode input_trig_seed;
    
    uint32_t seed;
    uint32_t seedMult;
    uint32_t pnRegister;
    
};

}


#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif


#endif  // PDSP_RANDOM_PRNOISEGEN_H_INCLUDED
