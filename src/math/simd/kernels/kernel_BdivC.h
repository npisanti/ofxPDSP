

#ifndef OFX_SIMDFLOATS_KERNEL_BDIVC_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_BDIVC_H_INCLUDED

#include "../functions.h"

namespace ofx {
    
    struct kernel_BdivC
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_BdivC() {};
        
        inline_f  void operator()(float* a, const float* b, const float* c) const {
            (*a) = (*b) / (*c) ;
        }
        
        inline_f  void block(float* a, const float* b, const float* c) const {
            
            // load elements
            f128 x0  = m_load(b);
            f128 y0  = m_load(c);
            f128 x4  = m_load(b+4);
            f128 y4  = m_load(c+4);

            // do the computations
            x0  = m_div(x0,  y0);
            x4  = m_div(x4,  y4); 
                      
            // store the results
            m_store(a,    x0);
            m_store(a+4,  x4);

        }

    };
        
}

#endif  // OFX_SIMDFLOATS_KERNEL_BDIVC_H_INCLUDED
