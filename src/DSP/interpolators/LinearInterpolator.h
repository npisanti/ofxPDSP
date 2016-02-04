
// LinearInterpolator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_INTEPOLATORS_LINEARINTERPOLATOR_H_INCLUDED
#define PDSP_INTEPOLATORS_LINEARINTERPOLATOR_H_INCLUDED

#include "Interpolator.h"



namespace pdsp {
    
    /*!
    @brief Really cheap interpolator.

    This interpolator performs linear interpolation between two values. WARNING! the given table must have an extra index as guard point at the end.
    */    
    class LinearInterpolator : public Interpolator{
        
    public:
        float interpolate(const float* table, const float &index, const int &tableLen) noexcept override;
        
        
        static LinearInterpolator linearInterpolator;
        
    };
    
    
}



#endif  // PDSP_INTEPOLATORS_LINEARINTERPOLATOR_H_INCLUDED
