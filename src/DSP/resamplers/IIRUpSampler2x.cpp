
#include "IIRUpSampler2x.h"

pdsp::IIRUpSampler2x::IIRUpSampler2x(int inputOversampleLevel) : UpSampler(inputOversampleLevel, inputOversampleLevel*2){

        for( int i=0; i<4; ++i){
            apf[i].x0 = 0.0;
            apf[i].x1 = 0.0;
            apf[i].x2 = 0.0;
            apf[i].y0 = 0.0;
            apf[i].y1 = 0.0;
            apf[i].y2 = 0.0;
        }
        
        apf[0].a = 0.07986642623635751;
        apf[1].a = 0.5453536510711322;
        apf[2].a = 0.28382934487410993;
        apf[3].a = 0.8344118914807379;
        
        oldout = 0.0;
}

pdsp::IIRUpSampler2x::IIRUpSampler2x() : IIRUpSampler2x(1){}

void pdsp::IIRUpSampler2x::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::IIRUpSampler2x::releaseResources () {}

void pdsp::IIRUpSampler2x::upsample(float* output, const float* input, const int &factor, const int &inputBufferSize) noexcept {
                
        for(int n=0; n<inputBufferSize; ++n){
                float yn = input[n];  
                uint32_t casted = *reinterpret_cast<uint32_t*> ( &yn );
                int exponent = casted & 0x7F800000;
                int aNaN = exponent < 0x7F800000;
                int aDen = exponent > 0;
                int result = casted * (aNaN & aDen );
                yn = *reinterpret_cast<float*> ( &result );
            
                output[n*2+1] = 0.0f;
                output[n*2] = yn;                  
        }
        //now the output has the zero stuffed input array inside
        
        for(int n=0; n<inputBufferSize*2; ++n){ //antialias-filtering
              
                double filterout;
                
                filterout = output[n];
                for(int i=0; i<2; ++i){
                        // shuffle inputs
                        apf[i].x2 = apf[i].x1;
                        apf[i].x1 = apf[i].x0;
                        apf[i].x0 = filterout;

                        //shuffle outputs
                        apf[i].y2=apf[i].y1;
                        apf[i].y1=apf[i].y0;

                        filterout = apf[i].x2 + ( (apf[i].x0-apf[i].y2) * apf[i].a );
                        
                        apf[i].y0 = filterout;
                }
                
                double newout = (filterout + oldout) * 0.5;

                filterout = output[n];
                for(int i=2; i<4; ++i){
                        // shuffle inputs
                        apf[i].x2 = apf[i].x1;
                        apf[i].x1 = apf[i].x0;
                        apf[i].x0 = filterout;

                        //shuffle outputs
                        apf[i].y2=apf[i].y1;
                        apf[i].y1=apf[i].y0;

                        filterout = apf[i].x2 + ( (apf[i].x0-apf[i].y2) * apf[i].a );
                        
                        apf[i].y0 = filterout;
                }                
                oldout = filterout;
                
                output[n] = newout;
                             
        }

}
