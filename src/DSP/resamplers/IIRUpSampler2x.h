
// IIRUpSampler2x.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_RESAMPLERS_IIRUPSAMPLER2X_H_INCLUDED
#define PDSP_RESAMPLERS_IIRUPSAMPLER2X_H_INCLUDED

#include "UpSampler.h"

namespace pdsp{
    /*!
    @brief Upsample to half the oversampling level.
    
    This resampler upsample the signal and filters it, removing aliasing components. It operates only doubling the oversample level, you need to chain more IIRUpSampler2x for reaching higher oversample levels.
    */        
class IIRUpSampler2x : public UpSampler {
        
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
        IIRUpSampler2x(int inputOversampleLevel);
        IIRUpSampler2x();
     
private: 

        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        
        void upsample(float* output, const float* input, const int &factor, const int &inputBufferSize) noexcept override;

        APFValues apf [4];
        double oldout;
};

}// pdsp namespace end



#endif  // PDSP_RESAMPLERS_IIRUPSAMPLER2X_H_INCLUDED
