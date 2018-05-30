
// IIRDownSampler2x.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED
#define PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED

#include "DownSampler.h"

namespace pdsp{
    /*!
    @brief Downsample to half the oversampling level.
    
    This downsampler filters the signal and then downsample it, removing aliasing components. It operates only halving the oversample level, you need to chain more IIRUpSampler2x for downsample from higher oversample levels.
    */        
class IIRDownSampler2x : public DownSampler {

    struct APFValues{
        double a;
        
        double x0;
        double x1;
        double x2;

        double y0;
        double y1;
        double y2;
    };
           
public:
        IIRDownSampler2x(int outputOversampleLevel);
        IIRDownSampler2x();

private: 
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        
        void downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept  override;

        APFValues apf [4];
        double oldout;
};

}//END NAMESPACE




#endif  // PDSP_RESAMPLERS_IIRDOWNSAMPLER2X_H_INCLUDED
