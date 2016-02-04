
// phazorShifter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_PHAZORSHIFTER_H_INCLUDED
#define PDSP_MATH_PHAZORSHIFTER_H_INCLUDED

#include "../functions.h"

namespace pdsp {
    
    struct shift_S_kernel
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        shift_S_kernel(float shift) : shift(shift) {};
        
        inline void operator()(float* dest, const float* src) const {
            float shifted = (*src) + shift;
            
            float shift_floor = static_cast<float>(static_cast<int>(shifted));
            if(shifted<0.0f){
                shift_floor -= 1.0f;
            }
            
            (*dest) = shifted - shift_floor;
        }
        
        inline void block(float* dest, const float* src) const {
            
            // load 8 data elements
            ofx::f128 x0 = ofx::m_load(src);
            ofx::f128 x4 = ofx::m_load(src+4);
            
            // do the computations
            x0 = ofx::m_add1(x0, shift);
            x4 = ofx::m_add1(x4, shift);
            
            ofx::f128 x0floor = ofx::m_floor(x0);
            ofx::f128 x4floor = ofx::m_floor(x4);
            
            x0 = ofx::m_sub(x0, x0floor);
            x4 = ofx::m_sub(x4, x4floor);
            
            // store the results
            ofx::m_store(dest, x0);
            ofx::m_store(dest+4, x4);
            
        }
        
    private:
        float shift;
    };
    
    
    
    struct shift_B_kernel
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        shift_B_kernel() {};
        
        inline void operator()(float* dest, const float* src, const float* shift) const {
            
            float shifted = (*src) + (*shift);
            
            float shift_floor = static_cast<float>(static_cast<int>(shifted));
            if(shifted<0.0f){
                shift_floor -= 1.0f;
            }
            
            (*dest) = shifted - shift_floor;
        }
        
        inline void block(float* dest, const float* src, const float* shift) const {
            
            // load 8 data elements
            ofx::f128 x0 = ofx::m_load(src);
            ofx::f128 s0 = ofx::m_load(shift);
            ofx::f128 x4 = ofx::m_load(src+4);
            ofx::f128 s4 = ofx::m_load(shift+4);
            
            // do the computations
            x0 = ofx::m_add(x0, s0);
            x4 = ofx::m_add(x4, s4);
            
            ofx::f128 x0floor = ofx::m_floor(x0);
            ofx::f128 x4floor = ofx::m_floor(x4);
            
            x0 = ofx::m_sub(x0, x0floor);
            x4 = ofx::m_sub(x4, x4floor);
            
            // store the results
            ofx::m_store(dest, x0);
            ofx::m_store(dest+4, x4);
            
        }
    };
    

    
    inline_f void vect_phazorShiftS (float* dest, const float S, const int len){
        ofx::transform_n(dest, dest, len, shift_S_kernel(S));
    }
    
    inline_f void vect_phazorShiftS (float* dest, const float* src, const float S, const int len){
        ofx::transform_n(dest, src, len, shift_S_kernel(S));
    }
    
    
    inline_f void vect_phazorShiftB (float* dest, const float* B, const int len){
        ofx::transform3_n( dest, dest, B, len, shift_B_kernel() );
    }   
    
    inline_f void vect_phazorShiftB (float* dest, const float* src, const float* B, const int len){
        ofx::transform3_n( dest, src, B, len, shift_B_kernel() );
    }   
    
}


#endif  // PDSP_MATH_PDSP_MATH_PHAZORSHIFTER_H_INCLUDED
