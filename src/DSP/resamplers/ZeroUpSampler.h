
// ZeroUpSampler.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_RESAMPLERS_ZEROUPSAMPLER_H_INCLUDED
#define PDSP_RESAMPLERS_ZEROUPSAMPLER_H_INCLUDED

#include "UpSampler.h"

namespace pdsp{
    /*!
    @brief Upsample the signal without filtering, just copying values.
    
    This upsampler operates in trivial manner, and as it doesn't filter the signal it introduces aliasing into audio, so it is suitable to just modulation signals.
    */           
class ZeroUpSampler : public UpSampler {
        
public:
        ZeroUpSampler(int inputOversample, int outputOversample);
        ZeroUpSampler();
     
private: 

        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        
        void upsample(float* output, const float* input, const int &factor, const int &inputBufferSize) noexcept override;
        
};

}//END NAMESPACE




#endif  // PDSP_RESAMPLERS_ZEROUPSAMPLER_H_INCLUDED
