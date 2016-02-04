
// Interpolator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_INTEPOLATORS_INTERPOLATOR_H_INCLUDED
#define PDSP_INTEPOLATORS_INTERPOLATOR_H_INCLUDED


#include "../pdspCore.h"

namespace pdsp {
    
/*!
@brief Abstract class for implementing interpolation algorithm.

Interpolator rapresents an interpolation algorithm with all the needed variables. Usually an interpolator is managed by an InterpolatorShell that create and delete Interpolator instances. Usually bu pdsp internal convention the table passed to an Interpolator has an extra guard point at the end.

*/

class Interpolator{

public:
    /*!
    @brief this is the interpolation algorithm to implement 
    @param[in] table an array of values. The values to interpolate are at the given index.
    @param[in] index the index inside the table of values
    @param[in] tableLen the length of the given table

    */
    virtual float interpolate(const float* table, const float &index, const int &tableLen) noexcept=0;

    /*!
    @brief this function should return true if the subclass that implements it is stateful (for example allpass interpolators), false if it is stateless
    */   
    virtual bool hasState(){
        return false;
    }

    /*!
    @brief this function should reset a stateful interpolator (for example allpass interpolators) to its init state
    */ 
    virtual void reset(){};
    
    virtual ~Interpolator(){};
    
};



    
    
}



#endif  // PDSP_INTEPOLATORS_INTERPOLATOR_H_INCLUDED
