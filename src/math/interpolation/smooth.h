/*
  from 
	Interpolation Algorithms Library
		v 1.1

	Cockos Reaper forum, user "SaulT"
		
*/

#ifndef PDSP_MATH_SMOOTH_H_INCLUDED
#define PDSP_MATH_SMOOTH_H_INCLUDED

#include "../functions.h"

namespace pdsp{

    inline_f float interpolate_smooth(const float x1, const float x2, const float mu){
    
        float mu2 = mu * mu * (3 - (2 * mu));
        return (x1 + (x2 - x1) * mu2);
    
    }


}

#endif  // PDSP_MATH_SMOOTH_H_INCLUDED
