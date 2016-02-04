
// complex_mult_add.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_COMPLEX_MULT_ADD_H_INCLUDED
#define PDSP_MATH_COMPLEX_MULT_ADD_H_INCLUDED

#include "../functions.h"

namespace pdsp{

//len is a multiple of 4
inline void vect_cmadd( float* addR, float* addI, const float*aR, const float*aI, const float*bR, const float* bI, int len){

        int n=0;
        int maxSimd = ROUND_DOWN(len, 4);
        
        for (; n<maxSimd; n+=4) {
                
                ofx::f128 ar0 = ofx::m_load(aR + n);
                ofx::f128 br0 = ofx::m_load(bR + n);
                ofx::f128 ai0 = ofx::m_load(aI + n);
                ofx::f128 bi0 = ofx::m_load(bI + n);
                ofx::f128 addr = ofx::m_load(addR + n);
                ofx::f128 addi = ofx::m_load(addI + n);
                
                ofx::f128 temp1 = ofx::m_mul(ar0, br0);
                ofx::f128 temp2 = ofx::m_mul(ai0, bi0);
                temp1 = ofx::m_sub(temp1, temp2);
                
                addr = ofx::m_add(addr, temp1);
                
                ofx::m_store(addR + n, addr);
                
                temp1 = ofx::m_mul(ar0, bi0);
                temp2 = ofx::m_mul(ai0, br0);
                temp1 = ofx::m_add(temp1, temp2);
                
                addi = ofx::m_add(addi, temp1);
                
                ofx::m_store(addI + n, addi);
        }    
        
        for (; n<len; ++n ) {
               addR[n] += aR[n] * bR[n] - aI[n] * bI[n]; 
               addI[n] += aR[n] * bI[n] + aI[n] * bR[n]; 
        }   
}


}//NAMESPACE END

#endif  // PDSP_MATH_COMPLEX_MULT_ADD_H_INCLUDED
