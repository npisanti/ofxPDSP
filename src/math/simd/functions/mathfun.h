

#ifndef OFX_SIMDFLOATS_MATHFUN_H_INCLUDED
#define OFX_SIMDFLOATS_MATHFUN_H_INCLUDED

#include "../core/flags.h"

#if defined( OFX_SIMD_USE_SSE2 )
#include "mathfun/mathfun_SSE2.h"

#elif defined( OFX_SIMD_USE_NEON )
#include "mathfun/mathfun_NEON.h"


        
#endif

#endif  // OFX_SIMDFLOATS_MATHFUN_H_INCLUDED
