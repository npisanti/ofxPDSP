
// FFTWorker.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_HELPERS_USESFFT_H_INCLUDED
#define PDSP_HELPERS_USESFFT_H_INCLUDED

#include "../pdspConstants.h"
#include "../../math/header.h"

namespace pdsp{
        
    /*!
    @brief Class to use an FFT Implementation 
    
    This manage the instantiation and initialization of an FFT algorithm to be used into subclasses. It use the AudioFFT library so the implementation is chosen at compile time setting the right flag. 
    */        

class FFTWorker {
public:       
    FFTWorker();
    ~FFTWorker();
    
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
    int blockSize;
    int complexSize;
    audiofft::AudioFFTBase* fftImplementation;
    int lastBufferSize;

};
    

        
}//END NAMESPACE

#endif  // PDSP_HELPERS_USESFFT_H_INCLUDED
