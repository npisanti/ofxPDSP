
#include "IIRUpSampler2x.h"

pdsp::IIRUpSampler2x::IIRUpSampler2x(int inputOversampleLevel) : UpSampler(inputOversampleLevel, inputOversampleLevel*2){
        AAFilter = new CHalfBandFilter(4, false);
}

pdsp::IIRUpSampler2x::IIRUpSampler2x() : IIRUpSampler2x(1){}


pdsp::IIRUpSampler2x::~IIRUpSampler2x(){
        delete AAFilter;
}

void pdsp::IIRUpSampler2x::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::IIRUpSampler2x::releaseResources () {}

void pdsp::IIRUpSampler2x::upsample(float* output, const float* input, const int &factor, const int &inputBufferSize) noexcept {
                
        ofx_Aeq_Zero(output, inputBufferSize*factor);
        for(int n=0; n<inputBufferSize; ++n){
                output[n*2] = input[n];                    
        }
        //now the output has the zero stuffed input array inside
        
        for(int n=0; n<inputBufferSize*2; ++n){ //antialias-filtering
                output[n] = AAFilter->process(  output[n]  );                
        }

}
