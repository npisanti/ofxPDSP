

#include "activate.h"

#include "flags.h"


void ofx_activate_denormal_flush(){

#ifdef OFX_SIMD_USE_SSE
        //mode to flush denormals values, needed for fast multiplication
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
#endif
};
    
   
