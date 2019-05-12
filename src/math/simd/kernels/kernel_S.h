

#ifndef OFX_SIMDFLOATS_KERNEL_S_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_S_H_INCLUDED

#include "../functions.h"

namespace ofx{
    
    struct kernel_S
    {
    public:
        enum { BLOCK_SIZE = 16 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_S(float s) : s(s){};
        
        inline_f  void operator()(float* a, const float* b) const {
            (*a) = s;
        }
        
        inline_f  void block(float* a, const float* b) const {
            
            m_store_scalar (a,    s);
            m_store_scalar (a+4,  s);
            m_store_scalar (a+8,  s);
            m_store_scalar (a+12, s);
            
        }
    protected:
        float s;
        
    };
    
}


#endif  // OFX_SIMDFLOATS_KERNEL_S_H_INCLUDED
