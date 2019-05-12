

#ifndef OFX_SIMDFLOATS_TERNARYOP_H_INCLUDED
#define OFX_SIMDFLOATS_TERNARYOP_H_INCLUDED

#include "simd_wrapper.h"


namespace ofx{
    
    inline_f f128 m_ternary_eq(const f128 & a, const f128 & b, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp_eq(a, b);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_gt(const f128 & a, const f128 & b, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp_gt(a, b);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_ge(const f128 & a, const f128 & b, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp_ge(a, b);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_lt(const f128 & a, const f128 & b, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp_lt(a, b);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_le(f128 a, const f128 & b, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp_le(a, b);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    
    //compare to a scalar
    
    inline_f f128 m_ternary_eq1(const f128 & a, const float s, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp1_eq(a, s);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_gt1(const f128 & a, const float s, const f128 &true4, const f128 &false4){
        f128 truemask = m_cmp1_gt(a, s);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_ge1(const f128 & a, const float s, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp1_ge(a, s);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_lt1(const f128 & a, const float s, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp1_lt(a, s);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    inline_f f128 m_ternary_le1(const f128 & a, const float s, const f128 & true4, const f128 & false4){
        f128 truemask = m_cmp1_le(a, s);
        f128 falsemask = m_and_nota(truemask, false4);
        truemask = m_and(truemask, true4);
        return m_or(truemask, falsemask);
    }
    
    
    
    
}



#endif  // OFX_SIMDFLOATS_TERNARYOP_H_INCLUDED
