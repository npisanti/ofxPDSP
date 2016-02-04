

#ifndef PDSP_MATH_LINEAR_H_INCLUDED
#define PDSP_MATH_LINEAR_H_INCLUDED

#include "../functions.h"

namespace pdsp{

    
    inline_f float interpolate_linear(const float x1, const float x2, const float mu){
      
        return ((x1 * (1.0f-mu)) + (x2 * mu)  );
        
    }
}

#endif  // PDSP_MATH_LINEAR_H_INCLUDED
