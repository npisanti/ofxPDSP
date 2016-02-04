
// ZeroDownSampler.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_RESAMPLERS_ZERODOWNSAMPLER_H_INCLUDED
#define PDSP_RESAMPLERS_ZERODOWNSAMPLER_H_INCLUDED

#include "DownSampler.h"

namespace pdsp{
    /*!
    @brief Downsample the signal without filtering, just removing values.
    
    This downsampler operates in trivial manner, and as it doesn't filter the signal it introduces aliasing into audio, so it is suitable to just modulation signals.
    */          
class ZeroDownSampler : public DownSampler {
        
public:
        ZeroDownSampler(int inputOversample, int outputOversample);
        ZeroDownSampler();
     
private: 

        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        
        void downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept  override;
        
};

}//END NAMESPACE


#endif  // PDSP_RESAMPLERS_ZERODOWNSAMPLER_H_INCLUDED
