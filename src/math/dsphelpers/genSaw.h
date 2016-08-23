
// genSaw.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_GENSAW_H_INCLUDED
#define PDSP_MATH_GENSAW_H_INCLUDED

#include "../functions.h"

namespace pdsp {
    
    inline_f void genSaw(float* outputBuffer, const float* phaseBuffer, int len){
  
      
        int n=0;
        
#ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            ofx::f128 X1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 X2 = ofx::m_load(phaseBuffer + n + 4);
            
            ofx::f128 result1 = ofx::m_mul1(X1, 2.0f);
            ofx::f128 result2 = ofx::m_mul1(X2, 2.0f);
            result1 = ofx::m_sub1(result1, 1.0f);
            result2 = ofx::m_sub1(result2, 1.0f);
            
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
        }
#else
        
        for (; n<len; ++n) {
            outputBuffer[n] = (phaseBuffer[n] * 2.0f) - 1.0f;
        }
#endif

    }
    
    
}



#endif  // PDSP_MATH_GENSAW_H_INCLUDED
