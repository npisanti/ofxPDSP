
// DownSampler.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_RESAMPLERS_DOWNSAMPLER_H_INCLUDED
#define PDSP_RESAMPLERS_DOWNSAMPLER_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    /*!
    @brief Abstract class to implement downsampling resamplers.

    */
class DownSampler : public Unit{

public:
    DownSampler(int inputOversample, int outputOversample);
        
    /*!
    @brief Changes input and output oversampling levels
    @param[in] inputOversample new input oversample level
    @param[in] outputOversample new output oversample level
    */        
    void setOversampleLevels(int inputOversample, int outputOversample);

protected:
    virtual void downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept =0;
    
private:
    void process (int bufferSize) noexcept override;    
    int     oversampleFactor;        

    InputNode input;
    OutputNode output;
};    


}//END NAMESPACE



#endif  // PDSP_RESAMPLERS_DOWNSAMPLER_H_INCLUDED
