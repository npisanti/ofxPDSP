/*
  from
	Interpolation Algorithms Library
		v 1.1

	Cockos Reaper forum, user "SaulT"

*/

#ifndef PDSP_MATH_HERMITE_H_INCLUDED
#define PDSP_MATH_HERMITE_H_INCLUDED

#include "../functions.h"


namespace pdsp{

    inline_f float interpolate_hermite(const float* table, const float index){

        int index_int = static_cast<int> (index);
        float mu = index - index_int;
        float x0 = table[index_int-1];
        float x1 = table[index_int];
        float x2 = table[index_int+1];
        float x3 = table[index_int+2];

        float a = ((3 * (x1- x2)) - x0 + x3) * 0.5f;
        float b = x2 + x2 + x0 - (5*x1 + x3) * 0.5f;
        float c = (x2 - x0) * 0.5f;
        float d = mu;
        return (((a*d+b)*d+c)*d+x1);

    }

}




#endif  // PDSP_MATH_HERMITE_H_INCLUDED
