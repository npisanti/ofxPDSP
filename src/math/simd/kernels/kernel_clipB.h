
#ifndef OFX_SIMDFLOATS_KERNEL_CLIPB_H_INCLUDED
#define OFX_SIMDFLOATS_KERNEL_CLIPB_H_INCLUDED

#include "../functions.h"

namespace ofx {
    
    struct kernel_clipB
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        kernel_clipB(float lo, float hi) : lo(lo), hi(hi) {};
        
        inline_f  void operator()(float* a, const float* b) const {
            if( (*b)<lo ){
                (*a) = lo;
            }else if( (*b)>hi ){
                (*a) = hi;
            }else{
                (*a) = (*b);
            }
        }
        
        inline_f  void block(float* a, const float* b) const {
            
            // load elements 
            f128 x0  = m_load(b);
            f128 x4  = m_load(b+4);
            
            //do computations
            x0 = m_clip(x0, lo, hi);
            x4 = m_clip(x4, lo, hi);
            
            // store the results
            m_store(a,    x0);
            m_store(a+4,  x4);
        }
        
    protected:
        float lo;
        float hi;
    };

}

#endif // OFX_SIMDFLOATS_KERNEL_CLIPB_H_INCLUDED

