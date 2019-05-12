

#ifndef OFX_SIMDFLOATS_TEMPLATEDTRANSFORM_H_INCLUDED
#define OFX_SIMDFLOATS_TEMPLATEDTRANSFORM_H_INCLUDED

#include "../core/inlines.h"

#include "../core/flags.h"

#include "../kernels/kernel_S.h"
#include "../kernels/kernel_Zero.h"
#include "../kernels/inline_ramp.h"
#include "../kernels/kernel_Badd_CmulS.h"
#include "../kernels/inline_logSB.h"
#include "../kernels/inline_powSB.h"
#include "../kernels/kernel_1divB.h"
#include "../kernels/kernel_absB.h"
#include "../kernels/kernel_B.h"
#include "../kernels/kernel_BmulC.h"
#include "../kernels/kernel_BmulS.h"
#include "../kernels/kernel_BaddC.h"
#include "../kernels/kernel_BaddS.h"
#include "../kernels/kernel_BsubC.h"
#include "../kernels/kernel_BsubS.h"
#include "../kernels/kernel_clipB.h"
#include "../kernels/kernel_maxBC.h"
#include "../kernels/kernel_minBC.h"
#include "../kernels/kernel_negB.h"
#include "../kernels/kernel_sqrtB.h"
#include "../kernels/kernel_SsubB.h"
#include "../kernels/kernel_sinB.h"
#include "../kernels/kernel_cosB.h"
#include "../kernels/kernel_tanB.h"
#include "../kernels/kernel_expB.h"
#include "../kernels/kernel_logB.h"
#include "../kernels/kernel_minBS.h"
#include "../kernels/kernel_maxBS.h"
#include "../kernels/kernel_BdivC.h"
#include "../kernels/kernel_floorB.h"

namespace ofx {
    
#if defined( OFX_SIMD_USE_SSE ) || defined( OFX_SIMD_USE_NEON )
    
    template<class BufferA, class BufferB, class Kernel>
    inline_f void transform_n(BufferA A, BufferB B, int len, Kernel kernel ){
        int n;
        // execute op on array elements block-wise
        for(n = 0; n < ROUND_DOWN(len, kernel.BLOCK_SIZE); n += kernel.BLOCK_SIZE)
            kernel.block(A + n, B + n);
        // execute the remaining array elements one by one
        for(; n < len; n++) kernel(A + n, B + n);
    }
    
    
    template<class Kernel>
    inline_f void unaligned_transform_n(float* A, const float* B, int start, int stopExclusive, Kernel kernel ){
        int n=0;
        int granularity = OFX_SIMD_ALIGNMENT_NUM / 4; //4 is sizeof(float)
        int simdMin =  ROUND_DOWN(start, granularity);
        int simdMax =  ROUND_DOWN(stopExclusive, kernel.BLOCK_SIZE);
    
        //execute unaligned one by one
        for(; n<simdMin; n++) kernel(A + n, B + n);
        // execute op on array elements block-wise
        for(; n < simdMax; n += kernel.BLOCK_SIZE)
            kernel.block(A + n, B + n);
        // execute the remaining array elements one by one
        for(; n < stopExclusive; n++) kernel(A + n, B + n);
    
    }
    
 
    template<class BufferA, class BufferB, class BufferC, class Kernel>
    inline_f void transform3_n(BufferA A, BufferB B, BufferC C, int len, Kernel kernel ){
        int n;
        // execute op on array elements block-wise
        for(n = 0; n < ROUND_DOWN(len, kernel.BLOCK_SIZE); n += kernel.BLOCK_SIZE)
            kernel.block( A+n, B+n, C+n );
        // execute the remaining array elements one by one
        for(; n < len; n++) kernel( A+n, B+n, C+n );
    }
    
#else
    
    //non simd operations
    template<class Kernel>
    inline_f void unaligned_transform_n(float* A, const float* B, int start, int stopExclusive, Kernel kernel ){
        int n=start;
        // execute the array elements one by one
        for(; n < stopExclusive; n++) kernel(A + n, B + n);
        
    }
    
    
    template<class BufferY, class BufferX, class Kernel>
    inline_f void transform_n(BufferY Y, BufferX X, int len, Kernel kernel ){
        for(int n = 0; n < len; n++) kernel(Y + n, X + n);
    }
    
    
    template<class BufferY, class BufferX, class BufferZ, class Kernel>
    inline_f void transform3_n(BufferY Y, BufferX X, BufferZ Z, int len, Kernel kernel ){
        for(int n = 0; n < len; n++) kernel( Y+n, X+n, Z+n );
    }
    
#endif

}


#endif  // OFX_SIMDFLOATS_TEMPLATEDTRANSFORM_H_INCLUDED
