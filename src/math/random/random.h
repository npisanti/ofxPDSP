
// random.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_RANDOM_H_INCLUDED
#define PDSP_MATH_RANDOM_H_INCLUDED

#include <random>
#include <limits>

namespace pdsp{
    
        float randomUnipolar();
        float randomUnipolarInclusive();
        float randomBipolar();
    
        int randomInt();
        int dice(int size);
        int randomBipolarInt(int min, int maxExclusive);        
    
        void seedGlobalRandom(int seed);
        
        bool chance(float value);
    
}

#endif  // PDSP_MATH_RANDOM_H_INCLUDED
