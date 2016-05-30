
// sanity_check.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_HELPERS_SANITYCHECK_H_INCLUDED
#define PDSP_MATH_HELPERS_SANITYCHECK_H_INCLUDED

#include <stdint.h>
#include "../functions.h"

namespace pdsp {
    
    // flush denormals, infinities and NaNs 
    // code mostlu by Urs Heckman from musicdsp.org =) 
    
    inline_f void sanity_check(float & sample) {
        
        uint32_t casted = *reinterpret_cast<uint32_t*> (&sample);
        int exponent = casted & 0x7F800000;
        int aNaN = exponent < 0x7F800000;
        int aDen = exponent > 0;
        int result = casted * (aNaN & aDen );
        sample = *reinterpret_cast<float*> ( &result );
        
    }
    
    void vect_sanity_check( float * samples, int len);

}


#endif // PDSP_MATH_HELPERS_SANITYCHECK_H_INCLUDED
