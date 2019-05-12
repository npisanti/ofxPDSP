

#ifndef OFX_SIMDFLOATS_FLAGS_H_INCLUDED
#define OFX_SIMDFLOATS_FLAGS_H_INCLUDED

//visual studio flags
#if (defined(_M_AMD64) || defined(_M_X64))
	#define __SSE2__
#elif _M_IX86_FP == 2
	#define __SSE2__
#endif

#ifdef __SSE2__
    #define OFX_SIMD_USE_SSE
    #define OFX_SIMD_USE_SSE2
    #define OFX_SIMD_USE_SIMD
    #define OFX_SIMD_ALIGNMENT_NUM 16
#endif

#if (__ARM_FEATURE_SIMD32 > 0)
    #ifndef __ARM_NEON
        #define __ARM_NEON
    #endif
#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    #define OFX_SIMD_USE_NEON
    #define OFX_SIMD_USE_SIMD
    #define OFX_SIMD_ALIGNMENT_NUM 16
#endif




#endif  // OFX_SIMDFLOATS_FLAGS_H_INCLUDED
