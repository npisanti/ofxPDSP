

#ifndef OFX_SIMDFLOATS_KERNEL_TANB_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_TANB_H_INCLUDED

#include "../functions.h"

namespace ofx {
    
    struct kernel_tanB
    {
    public:
        enum { BLOCK_SIZE = 4 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_tanB(){};
        
        inline_f  void operator()(float* a, const float* b) const {
            (*a) = tanf( (*b) );
        }
    
        
        inline_f  void block(float* a, const float* b) const {
            
            // load elements 
            f128 x0  = m_load(b);
            //f128 x4  = m_load(b+4);
        
            // do the computations
            x0  = m_tan(x0);
            //x4  = m_tan(x4);
            
            // store the results
            m_store(a,    x0);
            //m_store(a+4,  x4);

        }

    };
    

}


#endif  // OFX_SIMDFLOATS_KERNEL_TANB_H_INCLUDED
