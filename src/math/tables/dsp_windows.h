
// dsp_windows.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MATH_WINDOWS_H_INCLUDED
#define PDSP_MATH_WINDOWS_H_INCLUDED

#include "../functions.h"
#include "../../types.h"

namespace pdsp{
    

    

    float* windowTriangular( int len, int L_offset=0 );
    
    float* windowHann( int len );

    float* windowHamming( int len, double alpha=0.54);
    
    float* windowBlackman( int len, double alpha=0.16);
    
    float* windowBlackmanHarris( int len);

    float* windowSine( int len);

    float* windowWelch(int len);
    
    float* windowGaussian(int len, double alpha=0.5);
    
    float* windowTukey(int len, double alpha=0.5);
    
    float* window(Window_t type, int len);
    
}


#endif // PDSP_MATH_WINDOWS_H_INCLUDED
