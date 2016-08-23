
// genTriangle.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_GENTRIANGLE_H_INCLUDED
#define PDSP_MATH_GENTRIANGLE_H_INCLUDED

#include "../functions.h"


namespace pdsp {
    
    inline_f void genTriangle(float* outputBuffer, const float* phaseBuffer, int len){
        
      
        int n=0;
        
#ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            ofx::f128 result1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 result2 = ofx::m_load(phaseBuffer + n + 4);
            
            result1 = ofx::m_mul1(result1, 2.0f);
            result2 = ofx::m_mul1(result2, 2.0f);
            result1 = ofx::m_sub1(result1, 1.0f);
            result2 = ofx::m_sub1(result2, 1.0f);
            result1 = ofx::m_abs(result1);
            result2 = ofx::m_abs(result2);
            result1 = ofx::m_mul1(result1, 2.0f);
            result2 = ofx::m_mul1(result2, 2.0f);
            result1 = ofx::m_sub1(result1, 1.0f);
            result2 = ofx::m_sub1(result2, 1.0f);
                  
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
            
        }

#else
        for (; n<len; ++n) {
            outputBuffer[n] = ((fabs((phaseBuffer[n] * 2.0f) - 1.0f))*2.0f) - 1.0f;
        }
#endif        
        
        
    }
    
    inline_f void genTriangleUnipolar(float* outputBuffer, const float* phaseBuffer, int len){
        
        
        int n=0;
        
#ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            ofx::f128 result1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 result2 = ofx::m_load(phaseBuffer + n + 4);
            
            result1 = ofx::m_mul1(result1, 2.0f);
            result2 = ofx::m_mul1(result2, 2.0f);
            result1 = ofx::m_sub1(result1, 1.0f);
            result2 = ofx::m_sub1(result2, 1.0f);
            result1 = ofx::m_abs(result1);
            result2 = ofx::m_abs(result2);
   
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
            
        }
#else
        
        for (; n<len; ++n) {
            outputBuffer[n] = ((fabs((phaseBuffer[n] * 2.0f) - 1.0f))*2.0f) - 1.0f;
        }
#endif

    }
    
    
}





#endif  // PDSP_MATH_GENTRIANGLE_H_INCLUDED
