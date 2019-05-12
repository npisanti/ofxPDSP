

#ifndef OFX_SIMDFLOATS_KERNEL_BADDCMULS_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_BADDCMULS_H_INCLUDED

#include "../functions.h"

namespace ofx {
    
    struct kernel_Badd_CmulS
    {
    public:
        enum { BLOCK_SIZE = 16 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_Badd_CmulS(float s) : s(s) {};
        
        inline_f  void operator()(float* a, const float* b, const float* c) const {
            (*a) = (*b) + ((*c) * s);
        }
        
        inline_f  void block(float* a, const float* b, const float* c) const {
            
            // load elements
            f128 x0  = m_load(b);
            f128 y0  = m_load(c);
            f128 x4  = m_load(b+4);
            f128 y4  = m_load(c+4);
            f128 x8  = m_load(b+8);
            f128 y8  = m_load(c+8);
            f128 x12 = m_load(b+12);
            f128 y12 = m_load(c+12);
                        
            // do the computations
            y0  = m_mul1(y0,  s);
            x0  = m_add( x0,  y0);
            y4  = m_mul1(y4,  s);
            x4  = m_add( x4,  y4);
            y8  = m_mul1(y8,  s);
            x8  = m_add( x8,  y8);
            y12 = m_mul1(y12, s);
            x12 = m_add( x12, y12);  
                      
            // store the results
            m_store(a,    x0);
            m_store(a+4,  x4);
            m_store(a+8,  x8);
            m_store(a+12, x12); 
        }
        
    protected:
        float s;
    };
        
}

#endif  // OFX_SIMDFLOATS_KERNEL_BADDCMULS_H_INCLUDED
