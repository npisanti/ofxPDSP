

#ifndef PDSP_MATH_CUBIC_H_INCLUDED
#define PDSP_MATH_CUBIC_H_INCLUDED

#include "../functions.h"

namespace pdsp {

    inline_f float interpolate_cubic(const float x0, const float x1, const float x2, const float x3, const float mu) {

        float mu2 = mu * mu;
        float a = x3 - x2 + x1 - x0;
        float b = x0 - x1 - a;
        float c = x2 - x0;

        return x1 + a * mu2 * mu + b * mu2 + c * mu;

    }

}

#endif  // PDSP_MATH_CUBIC_H_INCLUDED
