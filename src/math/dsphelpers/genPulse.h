
// genPulse.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_GENSQUARE_H_INCLUDED
#define PDSP_MATH_GENSQUARE_H_INCLUDED

#include "../functions.h"


namespace pdsp {
    
    inline_f void genPulse(float* outputBuffer, const float* phaseBuffer, const float pw, int len){
        
        ofx::f128 positive = ofx::m_set1(1.0f);
        ofx::f128 negative = ofx::m_set1(-1.0f);
        ofx::f128 pw1 = ofx::m_set1(pw);
        
        
        int n=0;
        
        #ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);

        for (; n<maxSimd; n+=8) {
            
            ofx::f128 X1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 X2 = ofx::m_load(phaseBuffer + n + 4);
            
            ofx::f128 result1 = ofx::m_ternary_le(X1, pw1, positive, negative);
            ofx::f128 result2 = ofx::m_ternary_le(X2, pw1, positive, negative);
            
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
        }
        #endif
        
        for (; n<len; ++n) {
            if(phaseBuffer[n]<=pw){
                (*(outputBuffer+n)) = 1.0f;
            }else{
                (*(outputBuffer+n)) = -1.0f;
            }
        }
    }
    
    inline_f void genPulsePWM(float* outputBuffer, const float* phaseBuffer, const float* pwBuffer, int len){
        
        ofx::f128 positive = ofx::m_set1(1.0f);
        ofx::f128 negative = ofx::m_set1(-1.0f);
        
        
        int n=0;
        
        #ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            ofx::f128 X1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 pwm1 = ofx::m_load(pwBuffer+n);
            ofx::f128 X2 = ofx::m_load(phaseBuffer + n + 4);
            ofx::f128 pwm2 = ofx::m_load(pwBuffer + n + 4);
            
            ofx::f128 result1 = ofx::m_ternary_le(X1, pwm1, positive, negative);
            ofx::f128 result2 = ofx::m_ternary_le(X2, pwm2, positive, negative);
            
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
        }
        #endif
        
        for (; n<len; ++n) {
            if(phaseBuffer[n]<=pwBuffer[n]){
                (*(outputBuffer+n)) = 1.0f;
            }else{
                (*(outputBuffer+n)) = -1.0f;
            }
        }
    }
    
    
    
    inline_f void genPulseUnipolar(float* outputBuffer, const float* phaseBuffer, const float pw, int len){
        
        ofx::f128 positive = ofx::m_set1(1.0f);
        ofx::f128 zero = ofx::m_set1(0.0f);
        ofx::f128 pw1 = ofx::m_set1(pw);
        
        
        int n=0;
        
#ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            ofx::f128 X1 = ofx::m_load(phaseBuffer + n);
            ofx::f128 X2 = ofx::m_load(phaseBuffer + n + 4);
            
            ofx::f128 result1 = ofx::m_ternary_le(X1, pw1, positive, zero);
            ofx::f128 result2 = ofx::m_ternary_le(X2, pw1, positive, zero);
            
            ofx::m_store( outputBuffer + n , result1);
            ofx::m_store( outputBuffer + n +4, result2);
        }
#endif
        
        for (; n<len; ++n) {
            if(phaseBuffer[n]<=pw){
                (*(outputBuffer+n)) = 1.0f;
            }else{
                (*(outputBuffer+n)) = -1.0f;
            }
        }
    }
    
}



#endif  // PDSP_MATH_GENSQUARE_H_INCLUDED
