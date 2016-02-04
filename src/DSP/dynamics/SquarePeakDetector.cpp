
#include "SquarePeakDetector.h"


float pdsp::SquarePeakDetector::formula(const float &x) noexcept {
    return x*x;
}


void pdsp::SquarePeakDetector::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    
    int n = 0;
    int maxSimd = ROUND_DOWN(bufferSize, 16); 

#ifdef OFX_SIMD_USE_SIMD
    for ( ; n<maxSimd; n+=16){
        ofx::f128 x0  = ofx::m_load(input + n);
        ofx::f128 x4  = ofx::m_load(input + n + 4);
        ofx::f128 x8  = ofx::m_load(input + n + 8);
        ofx::f128 x12 = ofx::m_load(input + n + 12);
        
        x0  = ofx::m_mul(x0,  x0 );
        x4  = ofx::m_mul(x4,  x4 );
        x8  = ofx::m_mul(x8,  x8 );
        x12 = ofx::m_mul(x12, x12);
        
        ofx::m_store(output + n,      x0 ); 
        ofx::m_store(output + n + 4,  x4 ); 
        ofx::m_store(output + n + 8,  x8 ); 
        ofx::m_store(output + n + 12, x12); 
    }
#endif
    
    for( ; n<bufferSize; ++n){
        output[n] = input[n] * input[n];
    }
}
