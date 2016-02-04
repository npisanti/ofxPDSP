
#include "IIRDownSampler2x.h"


pdsp::IIRDownSampler2x::IIRDownSampler2x(int outputOversampleLevel) : DownSampler(outputOversampleLevel*2, outputOversampleLevel){
        AAFilter = new CHalfBandFilter(4, false);        
}

pdsp::IIRDownSampler2x::IIRDownSampler2x() : IIRDownSampler2x(1){}

pdsp::IIRDownSampler2x::~IIRDownSampler2x(){
        delete AAFilter;
}

void pdsp::IIRDownSampler2x::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::IIRDownSampler2x::releaseResources () {}

void pdsp::IIRDownSampler2x::downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept {

        for(int n=0; n<outputBufferSize; ++n){ //antialias-filtering
                float yn  = AAFilter->process( input[n*2]    );    
                //float yn2 = AAFilter->process( input[n*2 +1] ); 
                AAFilter->process( input[n*2 +1] ); 
                output[n] = yn;
        }
}

