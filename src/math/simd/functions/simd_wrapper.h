

#ifndef OFX_SIMDFLOATS_SIMD_WRAPPER_H_INCLUDED
#define OFX_SIMDFLOATS_SIMD_WRAPPER_H_INCLUDED

#include <cassert>
#include "../core/inlines.h"
#include "../core/flags.h"


#ifdef OFX_SIMD_USE_SSE2
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#endif


#ifdef OFX_SIMD_USE_NEON
#include <arm_neon.h>
#endif


#include <math.h>

namespace ofx{
    
#if defined( OFX_SIMD_USE_SSE2 )
        typedef __m128 f128;
        typedef __m128i i128;
        //typedef __m128d d128;
    
#elif defined( OFX_SIMD_USE_NEON )
        typedef float32x4_t f128;
        typedef int32x4_t i128;
        //typedef int64x2_t d128;
    
#else
        // no vectorization, define a struct with an array of 4 float to operate on
        // basically this should never happen
        struct float4{
                float data [4];
        };
        typedef struct float4 f128;
        struct int4{
                int data [4];
        };
        typedef struct int4 i128;
#endif
    
    //----------------------FUNCTION DECLARATIONS-----------------------------------------------

        inline_f f128 m_load (const float* p);
        inline_f f128 m_set1 (const float s);
        inline_f f128 m_set_zero ();

        inline_f void m_store(float* p, f128 result);
        inline_f void m_store_zero (float* p);
        inline_f void m_store_scalar (float* p, const float s);

        inline_f f128 m_add (f128 a, f128 b);
        inline_f f128 m_add1 (f128 a, const float s);
        inline_f f128 m_sub(f128 a, f128 b);
        inline_f f128 m_sub1 (f128 a, const float s);
        inline_f f128 m_1sub(const float s, f128 b);
        inline_f f128 m_mul (f128 a, f128 b);
        inline_f f128 m_mul1 (f128 a, const float s);
        inline_f f128 m_rcp(f128 a);
        inline_f f128 m_add_bs(f128 a, f128 b, const float s);
        inline_f f128 m_div (f128 a, f128 b);


        inline_f f128 m_abs(f128 a);
        inline_f f128 m_negate(f128 a);

        inline_f f128 m_max(f128 a, f128 b);
        inline_f f128 m_max1(f128 a, const float s);
        inline_f f128 m_min(f128 a, f128 b);
        inline_f f128 m_min1(f128 a, const float s);
        inline_f f128 m_clip(f128 a, const float lo, const float hi);

        inline_f f128 m_square_root(f128 a);
        inline_f f128 m_rcp_square_root(f128 a);


        inline_f f128 m_and(f128 a, f128 b);
        inline_f f128 m_and_nota(f128 a, f128 b);
        inline_f f128 m_or(f128 a, f128 b);
        inline_f f128 m_xor(f128 a, f128 b);

        inline_f f128 m_and1(f128 a, const float s);
        inline_f f128 m_and_nota_s1(f128 a, const float s);
        inline_f f128 m_or1(f128 a, const float s);
        inline_f f128 m_xor1(f128 a, const float s);


        inline_f f128 m_cmp_eq(f128 a, f128 b);
        inline_f f128 m_cmp_gt(f128 a, f128 b);
        inline_f f128 m_cmp_ge(f128 a, f128 b);
        inline_f f128 m_cmp_lt(f128 a, f128 b);
        inline_f f128 m_cmp_le(f128 a, f128 b);

        inline_f f128 m_cmp1_eq(f128 a, const float s);
        inline_f f128 m_cmp1_gt(f128 a, const float s);
        inline_f f128 m_cmp1_ge(f128 a, const float s);
        inline_f f128 m_cmp1_lt(f128 a, const float s);
        inline_f f128 m_cmp1_le(f128 a, const float s);




        inline_f i128 m_convt_fi(f128 a);
        inline_f f128 m_conv_if(i128 a);
        inline_f i128 m_intpow2(i128 x);

        inline_f f128 m_trunc(f128 a);

    
    //----------------------FUNCTION DEFINITIONS-----------------------------------------------
#if defined( OFX_SIMD_USE_SSE2 )
    //----------------------SSE-----------------------------------------------
    
        inline_f f128 m_load (const float* p){
#ifdef OFX_SIMD_ALIGNMENT_NUM
                return _mm_load_ps(p);
#else
                return _mm_loadu_ps(p);
#endif
        }

        inline_f f128 m_set1 (const float s){
                return _mm_set1_ps(s);
        }

        inline_f void m_store(float* p, f128 result){
#ifdef OFX_SIMD_ALIGNMENT_NUM
                _mm_store_ps(p, result);
#else
                _mm_storeu_ps(p, result);
#endif
        }

        inline_f f128 m_add (f128 a, f128 b){
                return _mm_add_ps(a, b);
        }

        inline_f f128 m_add1 (f128 a, const float s){
                return _mm_add_ps(a, _mm_set1_ps(s));
        }

        inline_f f128 m_sub(f128 a, f128 b){
                return _mm_sub_ps(a, b);
        }

        inline_f f128 m_sub1 (f128 a, const float s){
                return _mm_sub_ps(a, _mm_set1_ps(s));
        }

        inline_f f128 m_1sub(const float s, f128 b){
                return _mm_sub_ps(_mm_set1_ps(s), b );
        }


        inline_f f128 m_mul (f128 a, f128 b){
                return _mm_mul_ps(a, b);
        }

        inline_f f128 m_mul1 (f128 a, const float s){
                return _mm_mul_ps(a, _mm_set1_ps(s));
        }

        inline_f f128 m_set_zero (){
                return _mm_setzero_ps();
        }

        inline_f void m_store_zero (float* p){
#ifdef OFX_SIMD_ALIGNMENT_NUM
                _mm_store_ps(p, _mm_setzero_ps());
#else
                _mm_storeu_ps(p, _mm_setzero_ps());
#endif
        }

        inline_f void m_store_scalar (float* p, float s){
#ifdef OFX_SIMD_ALIGNMENT_NUM
                _mm_store_ps(p, _mm_set1_ps(s));
#else
                _mm_storeu_ps(p, _mm_set1_ps(s));
#endif
        }

        inline_f f128 m_add_bs(f128 a, f128 b, float s){
                return _mm_add_ps(a, _mm_mul_ps(b, _mm_set1_ps(s)));
        }

        inline_f f128 m_square_root(f128 a){
                return _mm_sqrt_ps(a);
        }

        inline_f f128 m_rcp_square_root(f128 a){
                return _mm_rsqrt_ps(a);
        }


        inline_f f128 m_abs(f128 a){
                return _mm_andnot_ps(_mm_set1_ps(-0.0f), a);
        }
        inline_f f128 m_negate(f128 a){
                return _mm_xor_ps(_mm_set1_ps(-0.0f), a);
        }

        inline_f f128 m_rcp(f128 a){
                return _mm_rcp_ps(a);
        }

        inline_f f128 m_div (f128 a, f128 b){
                return _mm_div_ps(a, b);
        }

        inline_f f128 m_max(f128 a, f128 b){
                return _mm_max_ps(a, b);
        }

        inline_f f128 m_max1(f128 a, const float s){
                return _mm_max_ps(a, _mm_set1_ps(s));
        }

        inline_f f128 m_min(f128 a, f128 b){
                return _mm_min_ps(a, b);
        }

        inline_f f128 m_min1(f128 a, const float s){
                return _mm_min_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_clip(f128 a, const float lo, const float hi){
                return _mm_max_ps(_mm_min_ps(a, _mm_set1_ps(hi)), _mm_set1_ps(lo));
        }

        inline_f f128 m_and(f128 a, f128 b){
                return _mm_and_ps(a, b);
        }

        inline_f f128 m_and_nota(f128 a, f128 b){
                return _mm_andnot_ps(a, b);
        }

        inline_f f128 m_or(f128 a, f128 b){
                return _mm_or_ps(a, b);
        }

        inline_f f128 m_xor(f128 a, f128 b){
                return _mm_xor_ps(a, b);
        }


        inline_f f128 m_and1(f128 a, const float s){
                return _mm_and_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_and_nota_s1(f128 a, const float s){
                return _mm_andnot_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_or1(f128 a, const float s){
                return _mm_or_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_xor1(f128 a, const float s){
                return _mm_xor_ps(a, _mm_set1_ps(s));
        }



        inline_f f128 m_cmp_eq(f128 a, f128 b){
                return _mm_cmpeq_ps(a, b);
        }

        inline_f f128 m_cmp_gt(f128 a, f128 b){
                return _mm_cmpgt_ps(a, b);
        }

        inline_f f128 m_cmp_ge(f128 a, f128 b){
                return _mm_cmpge_ps(a, b);
        }

        inline_f f128 m_cmp_lt(f128 a, f128 b){
                return _mm_cmplt_ps(a, b);
        }

        inline_f f128 m_cmp_le(f128 a, f128 b){
                return _mm_cmple_ps(a, b);
        }


        inline_f f128 m_cmp1_eq(f128 a, const float s){
                return _mm_cmpeq_ps(a, _mm_set1_ps(s));
        }

        inline_f f128 m_cmp1_gt(f128 a, const float s){
                return _mm_cmpgt_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_cmp1_ge(f128 a, const float s){
                return _mm_cmpge_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_cmp1_lt(f128 a, const float s){
                return _mm_cmplt_ps(a, _mm_set1_ps(s));
        }
        inline_f f128 m_cmp1_le(f128 a, const float s){
                return _mm_cmple_ps(a, _mm_set1_ps(s));
        }





        inline_f i128 m_convt_fi(f128 a){
                return _mm_cvttps_epi32(a);
        }

        inline_f f128 m_conv_if(i128 a){
                return _mm_cvtepi32_ps(a);
        }

        inline_f f128 m_trunc(f128 a){
                return _mm_cvtepi32_ps(_mm_cvttps_epi32(a));
        }
    
#elif defined( OFX_SIMD_USE_NEON )
    
    //----------------------NEON-----------------------------------------------
    inline_f f128 m_load (const float* p){
        return vld1q_f32(p);
    }
    
    inline_f f128 m_set1 (const float s){
        return vdupq_n_f32(s);
    }
    
    inline_f f128 m_set_zero (){
        return vdupq_n_f32(0);
    }
    
    
    inline_f void m_store(float* p, f128 result){
        vst1q_f32(p, result);
    }
    
    inline_f void m_store_zero (float* p){
        vst1q_f32(p, vdupq_n_f32(0));
    }
    
    inline_f void m_store_scalar (float* p, const float s){
        vst1q_f32(p, vdupq_n_f32(s));
    }
    
    inline_f f128 m_add (f128 a, f128 b){
        return vaddq_f32(a, b);
    }
    
    inline_f f128 m_add1 (f128 a, const float s){
        return vaddq_f32(a, vdupq_n_f32(s));
    }
    
    inline_f f128 m_sub(f128 a, f128 b){
        return vsubq_f32(a, b);
    }
    
    inline_f f128 m_sub1 (f128 a, const float s){
        return vsubq_f32(a, vdupq_n_f32(s));
    }
    
    inline_f f128 m_1sub(const float s, f128 b){
        return vsubq_f32(vdupq_n_f32(s), b);
    }
    
    inline_f f128 m_mul (f128 a, f128 b){
        return vmulq_f32(a, b);
    }
    
    inline_f f128 m_mul1 (f128 a, const float s){
        return vmulq_f32(a, vdupq_n_f32(s));
    }
    
    inline_f f128 m_rcp(f128 a){
        f128 recip = vrecpeq_f32(a);
        recip = vmulq_f32(recip, vrecpsq_f32(recip, a));
        return recip;
    }
    
    inline_f f128 m_div (f128 a, f128 b){
        f128 recip = vrecpeq_f32(b);
        recip = vmulq_f32(recip, vrecpsq_f32(recip, b));
        return vmulq_f32(a, recip);
    }
    
    inline_f f128 m_add_bs(f128 a, f128 b, const float s){
        return vaddq_f32(a, vmulq_f32(b, vdupq_n_f32(s)));
    }
    
    inline_f f128 m_abs(f128 a){
        return vabsq_f32(a);
    }
    
    inline_f f128 m_negate(f128 a){
        return vnegq_f32(a);
    }
    
    
    inline_f f128 m_square_root(f128 a){
        //test for accuracy, maybe it could change the sound of approssimated non-linear processing
        f128 rcp_square = vrsqrteq_f32(a);
        f128 squareroot = vrecpeq_f32(rcp_square);
        return squareroot;
    }
    
     inline_f f128 m_rcp_square_root(f128 a){
        return vrsqrteq_f32(a);
    }
    
    
    inline_f f128 m_max(f128 a, f128 b){
        return vmaxq_f32(a, b);
    }
    
    inline_f f128 m_max1(f128 a, const float s){
        return vmaxq_f32(a, vdupq_n_f32(s));
    }
    
    inline_f f128 m_min(f128 a, f128 b){
        return vminq_f32(a, b);
    }
    
    inline_f f128 m_min1(f128 a, const float s){
        return vminq_f32(a, vdupq_n_f32(s));
    }
    
    inline_f f128 m_clip(f128 a, const float lo, const float hi){
        return vminq_f32( vmaxq_f32( a, vdupq_n_f32(lo) ), vdupq_n_f32(hi) );
    }
    
    inline_f f128 m_and(f128 a, f128 b){
        return (f128) vandq_s32( (i128)a, (i128)b );
    }
    
    inline_f f128 m_and_nota(f128 a, f128 b){
        return (f128) vbicq_s32( (i128)b , (i128)a );
    }
    
    inline_f f128 m_or(f128 a, f128 b){
        return (f128) vorrq_s32( (i128)a, (i128)b );
    }
    
    inline_f f128 m_xor(f128 a, f128 b){
        return (f128) veorq_s32( (i128)a, (i128)b );
    }
    
    inline_f f128 m_and1(f128 a, const float s){
        return (f128) vandq_s32( (i128)a, (i128)vdupq_n_f32(s) );
    }
    
    inline_f f128 m_and_nota_s1(f128 a, const float s){
        return (f128) vbicq_s32( (i128)a , (i128)vdupq_n_f32(s) );
    }
    
    inline_f f128 m_or1(f128 a, const float s){
        return (f128) vorrq_s32( (i128)a, (i128)vdupq_n_f32(s) );
    }
    
    inline_f f128 m_xor1(f128 a, const float s){
        return (f128) veorq_s32( (i128)a, (i128)vdupq_n_f32(s) );
    }
    
    
    
    inline_f f128 m_cmp_eq(f128 a, f128 b){
        return (f128)vceqq_f32(a, b);
    }
    
    inline_f f128 m_cmp_gt(f128 a, f128 b){
        return (f128)vcgtq_f32(a, b);
    }
    
    inline_f f128 m_cmp_ge(f128 a, f128 b){
        return (f128)vcgeq_f32(a, b);
    }
    
    inline_f f128 m_cmp_lt(f128 a, f128 b){
        return (f128)vcltq_f32(a, b);
    }
    
    inline_f f128 m_cmp_le(f128 a, f128 b){
        return (f128)vcleq_f32(a, b);
    }
    
    inline_f f128 m_cmp1_eq(f128 a, const float s){
        return (f128)vceqq_f32(a, vdupq_n_f32(s));
    }
    inline_f f128 m_cmp1_gt(f128 a, const float s){
        return (f128)vcgtq_f32(a, vdupq_n_f32(s));
    }
    inline_f f128 m_cmp1_ge(f128 a, const float s){
        return (f128)vcgeq_f32(a, vdupq_n_f32(s));
    }
    inline_f f128 m_cmp1_lt(f128 a, const float s){
        return (f128)vcltq_f32(a, vdupq_n_f32(s));
    }
    inline_f f128 m_cmp1_le(f128 a, const float s){
        return (f128)vcleq_f32(a, vdupq_n_f32(s));
    }

    
    
    
    inline_f i128 m_convt_fi(f128 a){
        return vcvtq_s32_f32(a);
    }
    
    inline_f f128 m_conv_if(i128 a){
        return vcvtq_f32_s32(a);
    }
    
    inline_f i128 m_intpow2(i128 x){
        return vshlq_s32( vdupq_n_s32(1), x);
    }
    
    inline_f f128 m_trunc(f128 a){
        return vcvtq_f32_s32(vcvtq_s32_f32(a));
    }
    
#else
    //----------------------NOT ACCELERATED-----------------------------------------------
    
    inline_f f128 m_load (const float* p){}
    inline_f f128 m_set1 (const float s){}
    inline_f f128 m_set_zero (){}
    
    inline_f void m_store(float* p, f128 result){}
    inline_f void m_store_zero (float* p){}
    inline_f void m_store_scalar (float* p, const float s){}
    
    inline_f f128 m_add (f128 a, f128 b){}
    inline_f f128 m_add1 (f128 a, const float s){}
    inline_f f128 m_sub(f128 a, f128 b){}
    inline_f f128 m_sub1 (f128 a, const float s){}
    inline_f f128 m_1sub(const float s, f128 b){}
    inline_f f128 m_mul (f128 a, f128 b){}
    inline_f f128 m_mul1 (f128 a, const float s){}
    inline_f f128 m_rcp(f128 a){}
    inline_f f128 m_add_bs(f128 a, f128 b, const float s){}
    
    inline_f f128 m_abs(f128 a){}
    inline_f f128 m_negate(f128 a){}
    
    inline_f f128 m_max(f128 a, f128 b){}
    inline_f f128 m_max1(f128 a, const float s){}
    inline_f f128 m_min(f128 a, f128 b){}
    inline_f f128 m_min1(f128 a, const float s){}
    inline_f f128 m_clip(f128 a, const float lo, const float hi){}
    
    inline_f f128 m_square_root(f128 a){}
    inline_f f128 m_rcp_square_root(f128 a){}
    
    
    inline_f f128 m_and(f128 a, f128 b){}
    inline_f f128 m_and_nota(f128 a, f128 b){}
    inline_f f128 m_or(f128 a, f128 b){}
    inline_f f128 m_xor(f128 a, f128 b){}
    
    inline_f f128 m_and1(f128 a, const float s){}
    inline_f f128 m_and_nota_s1(f128 a, const float s){}
    inline_f f128 m_or1(f128 a, const float s){}
    inline_f f128 m_xor1(f128 a, const float s){}
    
    
    inline_f f128 m_cmp_eq(f128 a, f128 b){}
    inline_f f128 m_cmp_gt(f128 a, f128 b){}
    inline_f f128 m_cmp_ge(f128 a, f128 b){}
    inline_f f128 m_cmp_lt(f128 a, f128 b){}
    inline_f f128 m_cmp_le(f128 a, f128 b){}
    
    inline_f f128 m_cmp1_eq(f128 a, const float s){}
    inline_f f128 m_cmp1_gt(f128 a, const float s){}
    inline_f f128 m_cmp1_ge(f128 a, const float s){}
    inline_f f128 m_cmp1_lt(f128 a, const float s){}
    inline_f f128 m_cmp1_le(f128 a, const float s){}
    
    
    
    
    inline_f i128 m_convt_fi(f128 a){}
    inline_f f128 m_conv_if(i128 a){}
    inline_f i128 m_intpow2(i128 x){}
    
    inline_f f128 m_trunc(f128 a){}
    
    
    inline_f f128 m_log(f128 x){}
    inline_f f128 m_exp(f128 x){}
    inline_f f128 m_pow2(f128 x){}
    inline_f f128 m_sin(f128 x) {} // any x
    inline_f f128 m_cos(f128 x) {} // any x
    inline_f void m_sincos(f128 x, f128* s, f128*c ) {}
    inline_f f128 m_tan(f128 x) {}
    
#endif
    
    
}


#endif  // OFX_SIMDFLOATS_SIMD_WRAPPER_H_INCLUDED
