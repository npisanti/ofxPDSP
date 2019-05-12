

#ifndef OFX_SIMDFLOATS_INLINE_RAMP_H_INCLUDED
#define OFX_SIMDFLOATS_INLINE_RAMP_H_INCLUDED

#include "../functions.h"
#include "../core/inlines.h"
#include "../core/flags.h"


namespace ofx{
    
    inline void inline_ramp(float* dest, const float start, const float inc, const int len ){

        int n=0;
        
#ifdef OFX_SIMD_USE_SIMD
        
        ALIGNPRE float init [8] ALIGNPOST;  //define alignment flag
        init[0] = start;
        init[1] = start + inc;
        init[2] = start + inc*2;
        init[3] = start + inc*3;
        init[4] = start + inc*4;
        init[5] = start + inc*5;
        init[6] = start + inc*6;
        init[7] = start + inc*7;
        
        f128 now = m_load(init);
        f128 now2= m_load(init+4);
        f128 inc128 = m_set1(inc);
        
        m_store(dest, now);
        m_store(dest+4, now2);
        n=8;

        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            now = m_add(now, inc128);
            now2 = m_add(now2, inc128);
            
            m_store( dest + n , now);
            m_store( dest + n +4, now2);
        }
#endif
        for (; n<len; ++n) {
            (*(dest+n)) = (*(dest+n-1) + inc);
        }

    }
    
    inline void unaligned_ramp(float* dest, const int start, const int stopExclusive, const float startValue, const float inc ){
        
        int n = 1;
        dest[0] = startValue;
        
#ifdef OFX_SIMD_USE_SIMD
        int granularity = OFX_SIMD_ALIGNMENT_NUM / 4; //4 is sizeof(float)
        int startOffsetted = start+4;
        int simdMin =  ROUND_DOWN(startOffsetted, granularity);
        int simdMax =  ROUND_DOWN(stopExclusive, 8);

        
        for (; n<simdMin; ++n) {
            (*(dest+n)) = (*(dest+n-1) + inc);
        }
        
        float simdStartValue = dest[n-1];
        
        ALIGNPRE float init [8] ALIGNPOST;  //define alignment flag
        init[0] = simdStartValue + inc;
        init[1] = simdStartValue + inc*2;
        init[2] = simdStartValue + inc*3;
        init[3] = simdStartValue + inc*4;
        init[4] = simdStartValue + inc*5;
        init[5] = simdStartValue + inc*6;
        init[6] = simdStartValue + inc*7;
        init[7] = simdStartValue + inc*8;
        
        f128 now = m_load(init);
        f128 now2= m_load(init+4);
        f128 inc128 = m_set1(inc);
        
        m_store(dest, now);
        m_store(dest+4, now2);
        
        n= n+8;

        for (; n<simdMax; n+=8) {
            now = m_add(now, inc128);
            now2 = m_add(now2, inc128);
            
            m_store( dest + n , now);
            m_store( dest + n +4, now2);
        }

#endif
        for (; n<stopExclusive; ++n) {
            (*(dest+n)) = (*(dest+n-1) + inc);
        }
        
    }
    
}




#endif  // OFX_SIMDFLOATS_INLINE_RAMP_H_INCLUDED
