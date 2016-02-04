
#include "HalfWavePeakDetector.h"


float pdsp::HalfWavePeakDetector::formula(const float &x) noexcept {
        if(x > 0.0f){
                return x;
        }else{
                return 0.0f;
        }
}


void pdsp::HalfWavePeakDetector::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
        
        int n = 0;
        int maxSimd = ROUND_DOWN(bufferSize, 16); 

#ifdef OFX_SIMD_USE_SIMD
        for ( ; n<maxSimd; n+=16){
                ofx::f128 x0  = ofx::m_load(input + n);
                ofx::f128 x4  = ofx::m_load(input + n + 4);
                ofx::f128 x8  = ofx::m_load(input + n + 8);
                ofx::f128 x12 = ofx::m_load(input + n + 12);
                
                x0  = ofx::m_max1(x0,  0.0f);
                x4  = ofx::m_max1(x4,  0.0f);
                x8  = ofx::m_max1(x8,  0.0f);
                x12 = ofx::m_max1(x12, 0.0f);
                
                ofx::m_store(output + n,      x0 ); 
                ofx::m_store(output + n + 4,  x4 ); 
                ofx::m_store(output + n + 8,  x8 ); 
                ofx::m_store(output + n + 12, x12); 
        }
#endif        
        for( ; n<bufferSize; ++n){
                if(input[n] > 0.0f){
                        output[n] = input[n];
                }else{
                        output[n] = 0.0f;
                } 
        }
}
