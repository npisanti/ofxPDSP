
#ifndef OFX_SIMDFLOATS_INLINE_LOGSB_H_INCLUDED
#define OFX_SIMDFLOATS_INLINE_LOGSB_H_INCLUDED

#include "../functions.h"
#include "../core/inlines.h"
#include "../core/flags.h"


namespace ofx{
    
    
    // log of B in S base
    inline_f void inline_logSB(float* A, const float S, const float* B, const int len ){
        
        int n=0;
        float logBaseMult_f = 1.0f/ logf(S);
        
#ifdef OFX_SIMD_USE_SIMD

        f128 logBaseMult = m_set1( logBaseMult_f );

        int maxSimd = ROUND_DOWN(len, 8);
        
        for (; n<maxSimd; n+=8) {
            
            // load elements 
            f128 x0  = m_load(B+n);
            f128 x4  = m_load(B+n+4);
         
            // do the computations
            x0  = m_log(x0);
            x4  = m_log(x4);
            x0  = m_mul(x0, logBaseMult);
            x4  = m_mul(x4, logBaseMult);
            
            // store the results
            m_store(A+n,    x0);
            m_store(A+n+4,  x4);

        }
        
#endif
        for (; n<len; ++n) {
            A[n] = logf(B[n]) * logBaseMult_f;
        }

    }
    
}//END NAMESPACE

#endif // OFX_SIMDFLOATS_INLINE_LOGSB_H_INCLUDED
