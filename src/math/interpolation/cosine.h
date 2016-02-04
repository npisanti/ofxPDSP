/*
  from 
	Interpolation Algorithms Library
		v 1.1

	Cockos Reaper forum, user "SaulT"
		
*/

#ifndef PDSP_MATH_COSINE_H_INCLUDED
#define PDSP_MATH_COSINE_H_INCLUDED

#include "../functions.h"

namespace pdsp{
    
    inline_f float interpolate_cosine(const float x1, const float x2, const float mu){
        
        float mu2 = (1 - cosf(mu * M_PI))/2;
        return (x1 * (1-mu2) + x2 * mu2);
     
    }
 
}



#endif  // PDSP_MATH_COSINE_H_INCLUDED
