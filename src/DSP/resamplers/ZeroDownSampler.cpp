
#include "ZeroDownSampler.h"


pdsp::ZeroDownSampler::ZeroDownSampler(int inputOversample, int outputOversample) : DownSampler(inputOversample, outputOversample){}
pdsp::ZeroDownSampler::ZeroDownSampler() : ZeroDownSampler(4, 1){}


void pdsp::ZeroDownSampler::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::ZeroDownSampler::releaseResources () {}

void pdsp::ZeroDownSampler::downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept {
        
        for(int n=0; n<outputBufferSize; ++n){
                output[n] = input[n*factor];
        }

}

