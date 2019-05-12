
#ifndef OFX_SIMDFLOATS_INLINE_POWSB_H_INCLUDED
#define OFX_SIMDFLOATS_INLINE_POWSB_H_INCLUDED

#include "../functions.h"
#include "../core/inlines.h"
#include "../core/flags.h"


namespace ofx{
    
    
    // S elevated to B
    inline_f void inline_powSB(float* A, const float S, const float* B, const int len ){

        int n=0;
        float powBaseMult_f = logf(S);

    
#ifdef OFX_SIMD_USE_SIMD
        int maxSimd = ROUND_DOWN(len, 8);
        f128 powBaseMult = m_set1( powBaseMult_f );
        for (; n<maxSimd; n+=8) {
            
            // load elements 
            f128 x0  = m_load(B+n);
            f128 x4  = m_load(B+n+4);
         
            // do the computations
            x0  = m_mul(x0, powBaseMult);
            x4  = m_mul(x4, powBaseMult);
            x0  = m_exp(x0);
            x4  = m_exp(x4);
            
            // store the results
            m_store(A+n,    x0);
            m_store(A+n+4,  x4);

        }

#endif
        for (; n<len; ++n) {
            //A[n] = expf( B[n] * powBaseMult_f );
            A[n] = powf( S, B[n] );
        }

    }
    
}//END NAMESPACE

#endif // OFX_SIMDFLOATS_INLINE_POWSB_H_INCLUDED
