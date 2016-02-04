

#include "ZeroUpSampler.h"


pdsp::ZeroUpSampler::ZeroUpSampler(int inputOversample, int outputOversample) : UpSampler(inputOversample, outputOversample){}
pdsp::ZeroUpSampler::ZeroUpSampler() : ZeroUpSampler(1, 4){}


void pdsp::ZeroUpSampler::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::ZeroUpSampler::releaseResources () {}

void pdsp::ZeroUpSampler::upsample(float* output, const float* input, const int &factor, const int &inputBufferSize) noexcept {
        
        switch(factor){
        case 2:
                for(int n=0; n<inputBufferSize; ++n){
                        output[n*2]      = input[n];
                        output[n*2 + 1 ] = input[n];                        
                }
                break;
        case 4:
                for(int n=0; n<inputBufferSize; ++n){
                        output[n*4]      = input[n];
                        output[n*4 + 1 ] = input[n]; 
                        output[n*4 + 2 ] = input[n];
                        output[n*4 + 3 ] = input[n];                        
                }
                break;
        default:
                for(int n=0; n<inputBufferSize; ++n){
                        for(int i=0; i<factor; ++i){
                                output[n*factor + i] = input[n];
                        }
                }
                break;
        }

}

