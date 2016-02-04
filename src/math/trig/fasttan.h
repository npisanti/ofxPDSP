
// fasttan.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_FASTTAN_H_INCLUDED
#define PDSP_MATH_FASTTAN_H_INCLUDED

#include "../functions.h"

namespace pdsp{
    //STILL TO DO, PLACEHOLDER IMPLEMENTATION    
    inline_f float fasttan(float angle){
        static const float halfpi = 1.5707963267948966f;
        return sin (angle) / sin (angle + halfpi);
    }
    
}

#endif  // PDSP_MATH_FASTTAN_H_INCLUDED
