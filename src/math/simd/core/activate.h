
#ifndef OFX_SIMDFLOATS_ACTIVATE_H_INCLUDED
#define OFX_SIMDFLOATS_ACTIVATE_H_INCLUDED


#include "flags.h"

#ifdef OFX_SIMD_USE_SSE
#include <mmintrin.h>
#include <xmmintrin.h>
#endif



// treats denormalized floats as zeroes when using SIMD operation on SSE cpus
// if you are using a NEON cpu you can also ignore this, it won't do anything as 
// flush to zero is the standard NEON behavior.
void ofx_activate_denormal_flush();
   




#endif  // OFX_SIMDFLOATS_ACTIVATE_H_INCLUDED
