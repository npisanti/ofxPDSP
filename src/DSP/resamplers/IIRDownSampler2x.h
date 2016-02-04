
// IIRDownSampler2x.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED
#define PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED

#include "BandLimit.h"
#include "DownSampler.h"

namespace pdsp{
    /*!
    @brief Downsample to half the oversampling level.
    
    This downsampler filters the signal and then downsample it, removing aliasing components. It operates only halving the oversample level, you need to chain more IIRUpSampler2x for downsample from higher oversample levels.
    */        
class IIRDownSampler2x : public DownSampler {
        
public:
        IIRDownSampler2x(int outputOversampleLevel);
        IIRDownSampler2x();
        ~IIRDownSampler2x();

private: 
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        
        void downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept  override;

        CHalfBandFilter* AAFilter;
};

}//END NAMESPACE




#endif  // PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED
