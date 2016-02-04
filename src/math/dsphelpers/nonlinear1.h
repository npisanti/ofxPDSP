
// nonlinear1.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MATH_NONLINEAR_H_INCLUDED
#define PDSP_MATH_NONLINEAR_H_INCLUDED


#include "../functions.h"


namespace pdsp{
    
    //nonlinear processing using improved inverse sigmoid
    //a = x + 0.16489087 * x^3 + 0.00985468 * x^5
    //return a / sqrt(1 + a * a)
    
    struct nonlinear1_kernel
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        nonlinear1_kernel() {};
        
        inline void operator()(float* dest, const float* src) const {
            float x = (*src);
            float xCube = x*x*x;
            float a = x + (0.16489087f * xCube) + (0.00985468f * xCube*x*x);
            (*dest) = (a / sqrtf(1.0f + a * a));
        }
        
        inline void block(float* dest, const float* src) const {
            
            // load 8 data elements
            ofx::f128 x0 = ofx::m_load(src);
            ofx::f128 x4 = ofx::m_load(src+4);
            
            // do the computations
            ofx::f128 xCube0 = ofx::m_mul(x0, x0); //x^2
            ofx::f128 xCube4 = ofx::m_mul(x4, x4); //x^2
            
            xCube0 = ofx::m_mul(xCube0, x0); // x^3
            xCube4 = ofx::m_mul(xCube4, x4); // x^3
            
            ofx::f128 b0 = ofx::m_mul1(xCube0, 0.16489087f);
            ofx::f128 b4 = ofx::m_mul1(xCube4, 0.16489087f);
            
            ofx::f128 c0 = ofx::m_mul(xCube0, x0);
            ofx::f128 c4 = ofx::m_mul(xCube4, x4);
            
            c0 = ofx::m_mul(c0, x0); //x^5
            c4 = ofx::m_mul(c4, x4); //x^5
            
            c0 = ofx::m_mul1(c0, 0.0098546f);
            c4 = ofx::m_mul1(c0, 0.0098546f);
            
            ofx::f128 a0 = ofx::m_add(b0, c0);
            ofx::f128 a4 = ofx::m_add(b4, c4);
            
            a0 = ofx::m_add(a0, x0);
            a4 = ofx::m_add(a4, x4);
            
            x0 = ofx::m_mul(a0, a0);
            x4 = ofx::m_mul(a4, a4);
            
            x0 = ofx::m_add1(x0, 1.0f);
            x4 = ofx::m_add1(x4, 1.0f);
            
            x0 = ofx::m_rcp_square_root(x0);
            x4 = ofx::m_rcp_square_root(x4);
            
            x0 = ofx::m_mul(a0, x0);
            x4 = ofx::m_mul(a4, x4);
            
            // store the results
            ofx::m_store(dest, x0);
            ofx::m_store(dest+4, x4);
            
        }
        
    //private:
        
    };
    
    
    inline_f void vect_nonlinear1 (float* dest, const int len){
        ofx::transform_n(dest, dest, len, nonlinear1_kernel());
    }
    
}






#endif  // PDSP_MATH_NONLINEAR_H_INCLUDED
