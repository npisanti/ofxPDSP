
#ifndef OFX_SIMDFLOATS_FLOOR_H_INCLUDED
#define OFX_SIMDFLOATS_FLOOR_H_INCLUDED

#include "simd_wrapper.h"

namespace ofx{

    
    inline_f f128 m_floor(f128 a){
        f128 floor = m_trunc(a);
        f128 mask = m_cmp1_lt(a, 0.0f);
        mask = m_and1(mask, -1.0f);
        return m_add(floor, mask);
    }
    
    
}



#endif  // OFX_SIMDFLOATS_FLOOR_H_INCLUDED
