

#ifndef OFX_SIMDFLOATS_ZERO_H_INCLUDED
#define OFX_SIMDFLOATS_ZERO_H_INCLUDED

#include "../functions.h"

namespace ofx {
    
    struct kernel_Zero
    {
    public:
        enum { BLOCK_SIZE = 16 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_Zero(){};
        
        inline_f  void operator()(float* a, const float* b) const {
            (*a) = 0.0f;
        }
        
        inline_f  void block(float* a, const float* b) const {
            //_mm_storeu_ps(y, _mm_setzero_ps());
            //store(y, set_zero());
            m_store_zero(a);
            m_store_zero(a+4);
            m_store_zero(a+8);
            m_store_zero(a+12);
        }
        
        
    };
}



#endif  // OFX_SIMDFLOATS_ZERO_H_INCLUDED
