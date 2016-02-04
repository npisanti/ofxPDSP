
// UsesFFT.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_HELPERS_USESFFT_H_INCLUDED
#define PDSP_HELPERS_USESFFT_H_INCLUDED

#include "../pdspConstants.h"
#include "../../math/header.h"

namespace pdsp{
        
    /*!
    @brief FFT Implementation 
    
    This manage the instantiation and initialization of an FFT algorithm to be used into subclasses. It use the AudioFFT library so the implementation is chosen at compile time setting the right flag. Warning! all the Units share the same  implementation. This is usually not a problem until you set the oversampling of one of the UsesFFT subclasses to a value different than 1 (no oversampling). In that case there will be conflict between the various initFFT() and the behavior is undefined. Long story short: don't oversample classes that UsesFFT .
    */        

class UsesFFT {
public:       
    UsesFFT();
    ~UsesFFT();
    
    /*!
    @brief inits the fft
    */    
    void initFFT(int bufferSize);
 
    /*!
    @brief returns the fft block size, usually the first power of 2 greater than the audio buffer size.
    */      
    int getFFTBlockSize() const;
    
    /*!
    @brief returns the fft block size == half the block size
    */      
    int getFFTComplexSize() const;
    
    /*!
    @brief performs FFT
    @param[in] inSignal signal to process (time domain)
    @param[out] re real part output (frequency domain)
    @param[out] im imaginary part output (frequency domain)
    */        
    void    FFT  (const float* inSignal, float* re, float* im);
    
    /*!
    @brief performs iFFT
    @param[out] outSignal signal output (time domain)
    @param[in] re real part input (frequency domain)
    @param[in] im imaginary part input (frequency domain)
    */       
    void    iFFT (float* outSignal, const float* re, const float* im);

private:
    static int blockSize;
    static int complexSize;
    static audiofft::AudioFFTBase* fftImplementation;
    static int lastBufferSize;
    static int unitsCount;
};
    

        
}//END NAMESPACE

#endif  // PDSP_HELPERS_USESFFT_H_INCLUDED
