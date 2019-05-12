
#ifndef OFX_SIMDFLOATS_VECT_OPS_H_INCLUDED
#define OFX_SIMDFLOATS_VECT_OPS_H_INCLUDED

#include <stdlib.h>

#include "vect_transform.h"
#include "../core/inlines.h"
#include <string.h>
 

// for each element a = b + c
inline void ofx_Aeq_BaddC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_BaddC());
}  

// for each element a = b + s
inline void ofx_Aeq_BaddS(float * A, const float * B, const float s, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_BaddS(s));
}  

// for each element a = b * c
inline void ofx_Aeq_BmulC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_BmulC());
}  

// for each element a = b * s
inline void ofx_Aeq_BmulS(float * A, const float * B, const float s, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_BmulS(s));
}  

// for each element a = b - c
inline void ofx_Aeq_BsubC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_BsubC());
}  

// for each element a = b - s
inline void ofx_Aeq_BsubS(float * A, const float * B, const float s, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_BsubS(s));
}  

// for each element a = b / c
inline void ofx_Aeq_BdivC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_BdivC());
}  

// for each element a = s - b 
inline void ofx_Aeq_SsubB(float * A, const float s, const float * B,  const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_SsubB(s));
}  

// for each element a = b + (c*s)
inline void ofx_Aeq_Badd_CmulS (float * A, const float * B, const float * C ,const float S, const int len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_Badd_CmulS(S));
}

// for each element a = 1/b
inline void ofx_Aeq_1divB(float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_1divB() );
}

// for each element a is the absolute value of b
inline void ofx_Aeq_absB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_absB());
}

// for each element a = -b
inline void ofx_Aeq_negB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_negB());
}

// for each element a is the square root of b
inline void ofx_Aeq_sqrtB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_sqrtB());
}

// for each element a = sin(b)
inline void ofx_Aeq_sinB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_sinB());
}

// for each element a = cos(b)
inline void ofx_Aeq_cosB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_cosB());
}

// for each element a = tan(b)
inline void ofx_Aeq_tanB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_tanB());
}

// for each element a is the natural logarithm of b 
inline void ofx_Aeq_logB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_logB());
}

// for each element a is the exponential value of b
inline void ofx_Aeq_expB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_expB());
}

// for each element a is b rounded downward
inline void ofx_Aeq_floorB (float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_floorB());
}

// for each element a is the logarithm of b in s base
inline void ofx_Aeq_logSB(float * A, const float s, const float * B, const int & len){
    ofx::inline_logSB( A, s, B, len );
}

// for each element a is s elevated to b
inline void ofx_Aeq_powSB(float * A, const float s, const float * B, const int & len){
    ofx::inline_powSB( A, s, B, len );
}

// for each element a is b clamped to rangeLo and rangeHi
inline void ofx_Aeq_clipB(float*A, const float* B, const float rangeLo, const float rangeHi, const int len){
    ofx::transform_n(A, B, len, ofx::kernel_clipB(rangeLo, rangeHi));
}

// for each element b and c are comparated and the highest is set as a
inline void ofx_Aeq_maxBC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_maxBC());
} 

// for each element b and c are comparated and the lowest is set as a
inline void ofx_Aeq_minBC(float * A, const float * B, const float * C, const int & len){
    ofx::transform3_n(A, B, C, len, ofx::kernel_minBC());
} 

// for each element b and s are comparated and the highest is set as a
inline void ofx_Aeq_maxBS(float * A, const float * B, const float S, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_maxBS(S));
} 

// for each element b and s are comparated and the lowest is set as a
inline void ofx_Aeq_minBS(float * A, const float * B, const float S, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_minBS(S));
} 


// fill the A array with numbers making a line from the start value to the end value
// BEWARE: the end value is not included in the line
inline void ofx_Aeq_line (float*A , const float start, const float end, const int & len){
    float inc = (end - start) / static_cast<float>(len);
    ofx::inline_ramp(A, start, inc, len);
}

// fill the A array with numbers starting from the start value and 
// incremented by the inc value for each position
inline void ofx_Aeq_ramp (float*A , const float start, const float inc, const int & len){
    ofx::inline_ramp(A, start, inc, len);
}

// set the element of A to zero
inline void ofx_Aeq_Zero(float* A, const int & len){
    ofx::transform_n(A, A, len, ofx::kernel_Zero());
}

// copy the element of B to A
inline void ofx_Aeq_B(float * A, const float * B, const int & len){
    ofx::transform_n(A, B, len, ofx::kernel_B());
}

// set the element of A to a scalar number
inline void ofx_Aeq_S(float*A, const float s, const int & len){
    ofx::transform_n(A, A, len, ofx::kernel_S(s));
}

// set the element of A in the given range to a given scalar
// BEWARE: the range as to be inside of the allocated space, index are not checked
inline void ofx_Aeq_S_range(float*A, const float S, const int start, const int stopExclusive){
    ofx::unaligned_transform_n(A, A, start, stopExclusive, ofx::kernel_S(S));
}

// set the element of A in the given range to zero
// BEWARE: the range as to be inside of the allocated space, index are not checked
inline void ofx_Aeq_Zero_range(float*A,  const int start, const int stopExclusive){
    ofx::unaligned_transform_n(A, A, start, stopExclusive, ofx::kernel_Zero());
}



#endif  // OFX_SIMDFLOATS_VECT_OPS_H_INCLUDED
