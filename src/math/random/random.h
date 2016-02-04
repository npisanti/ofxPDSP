
// random.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_RANDOM_H_INCLUDED
#define PDSP_MATH_RANDOM_H_INCLUDED

#include <random>
#include <limits>

namespace pdsp{
    
        float randomUnipolar();
        float randomBipolar();
    
        int randomInt();
        int randomUnipolarInt(int maxExclusive);
        int randomBipolarInt(int min, int maxExclusive);        
    
        void seedGlobalRandom(int seed);
    
}

#endif  // PDSP_MATH_RANDOM_H_INCLUDED
