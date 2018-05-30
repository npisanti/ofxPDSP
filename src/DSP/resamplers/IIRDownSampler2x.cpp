
#include "IIRDownSampler2x.h"


pdsp::IIRDownSampler2x::IIRDownSampler2x(int outputOversampleLevel) : DownSampler(outputOversampleLevel*2, outputOversampleLevel){

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

pdsp::IIRDownSampler2x::IIRDownSampler2x() : IIRDownSampler2x(1){}

void pdsp::IIRDownSampler2x::prepareUnit( int expectedBufferSize, double sampleRate ) {}
void pdsp::IIRDownSampler2x::releaseResources () {}

void pdsp::IIRDownSampler2x::downsample(float* output, const float* input, const int &factor, const int &outputBufferSize) noexcept {

        for(int n=0; n<outputBufferSize; ++n){ //antialias-filtering

                double filterout, newout;                
                
                filterout = input[n*2];
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
                
                newout = (filterout + oldout) * 0.5;

                filterout = input[n*2];
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
                
                
                filterout = input[n*2 + 1];
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
                
                newout = (filterout + oldout) * 0.5;

                filterout = input[n*2 + 1];
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

        }
}

