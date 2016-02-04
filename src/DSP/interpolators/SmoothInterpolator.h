
// SmoothInterpolator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_INTEPOLATORS_SMOOTHINTERPOLATOR_H_INCLUDED
#define PDSP_INTEPOLATORS_SMOOTHINTERPOLATOR_H_INCLUDED

#include "Interpolator.h"

namespace pdsp {
    
    /*!
    @brief Good sounding stateless interpolator.

    This interpolator performs an interpolation that is better sounding than LinearInterpolator but more expensive, still using just two points. WARNING! the given table must have an extra index as guard point at the end.
    */

    class SmoothInterpolator : public Interpolator{
    
    public:
        
        float interpolate(const float* table, const float &index, const int &tableLen) noexcept override;
         
        static SmoothInterpolator smoothInterpolator;
        
    };
    
}

#endif  // PDSP_INTEPOLATORS_SMOOTHINTERPOLATOR_H_INCLUDED
