

#ifndef OFX_SIMDFLOATS_KERNEL_BSUBS_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_BSUBS_H_INCLUDED

#include "../functions.h"

namespace ofx{
    
    struct kernel_BsubS
    {
    public:
        enum { BLOCK_SIZE = 16 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_BsubS(float s): s(s){};
        
        inline_f  void operator()(float* a, const float* b) const {
            (*a) = (*b) - s ;
        }
        
        inline_f  void block(float* a, const float* b) const {
            // load elements 
            f128 x0  = m_load(b);
            f128 x4  = m_load(b+4);
            f128 x8  = m_load(b+8);
            f128 x12 = m_load(b+12);           
            
            // do the computations
            x0  = m_sub1(x0, s);
            x4  = m_sub1(x4, s);
            x8  = m_sub1(x8, s);
            x12 = m_sub1(x12, s);
            
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





#endif  // OFX_SIMDFLOATS_KERNEL_BSUBS_H_INCLUDED
