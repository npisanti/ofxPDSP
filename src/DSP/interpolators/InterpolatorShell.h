
// InterpolatorShell.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_INTEPOLATORS_INTERPOLATORSHELL_H_INCLUDED
#define PDSP_INTEPOLATORS_INTERPOLATORSHELL_H_INCLUDED

#include "Interpolator.h"
#include "LinearInterpolator.h"
#include "SmoothInterpolator.h"

namespace pdsp
{

/*!
@brief Contains an Interpolator of variable type.
rhytms
This class contains a pointer to an Interpolator. You can choose between different interpolation algorithms using the changeInterpolator() method. By default linear interpolation is selected.
*/

class InterpolatorShell{
    
public:
    
    InterpolatorShell();
    InterpolatorShell(const InterpolatorShell& other);
    InterpolatorShell& operator= (const InterpolatorShell& other);
    ~InterpolatorShell();
    
    /*!
    @brief change the type of the Interpolator, if necessary allocate a new Interpolator and/or destroys the old Interpolator
    @param[in] type you can choose between different interpolators. Check types.h
    */
    void changeInterpolator(Interpolator_t type);
    
    /*!
    @brief reset the interpolator state if it is a stateful interpolator.
    */
    void resetInterpolator();
    
    /*!
    @brief perform interpolation at the given point using the set Interpolator and returns the interpolated value.
    @param[in] table an array of values. The values to interpolate are at the given index.
    @param[in] index the index inside the table of values
    @param[in] tableLen the length of the given table
    */
    
    inline float interpolate(const float* table, const float &index, const int &tableLen) noexcept{
            return interpolator->interpolate(table, index, tableLen);
    }
    
    /*!
    @brief the pointer to the used Interpolator
    */
    Interpolator* interpolator;
    
    /*!
    @brief the type to the used Interpolator
    */    
    Interpolator_t type;
    
};

} // pdsp namespace end




#endif  // PDSP_INTEPOLATORS_INTERPOLATORSHELL_H_INCLUDED
