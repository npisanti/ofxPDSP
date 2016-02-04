
// CRSlew.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_SLEW_H_INCLUDED
#define PDSP_CONTROL_SLEW_H_INCLUDED

#include "../pdspCore.h"
#include "../helpers/UsesSlew.h"

namespace pdsp{

/*!
@brief Slews inputs running at control rate.

Slews inputs running at control rate, if an input is running at audio rate just the first element of the buffer is sampled, so it's mostly useful to patch ValueNode to this unit, or to directly send floats to its input using the >> operator. When the program starts the slew will start from an init value, that is also settable. This unit has just one input and one output, so it has no in_ or out_ methods to select. For slewing Units running at audio rate you should use a OnePole lpf filter.

*/

class CRSlew : public Unit, public UsesSlew {

public:
        CRSlew();

        /*!
        @brief Sets the slew time and returns the unit ready to be patched
        @param[in] slewTime Slew time in milliseconds.
        */
        Patchable& set(float slewTime);

        /*!
        @brief Sets the slew time and a starting default value and returns the unit ready to be patched.
        @param[in] slewTime Slew time in milliseconds.
        @param[in] initValue Default starting value.
        */        
        Patchable& set(float slewTime, float initValue);
   
        /*!
        @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the control rate input to slew .
        */ 
        Patchable& in_signal();
        
        /*!
        @brief Sets "signal" as selected output and return this Unit ready to be patched. This is the default output. This is the slewed output.
        */ 
        Patchable& out_signal();
        
private:
        OutputNode output;
        InputNode  input;

        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        void process (int bufferSize) noexcept override;

};    

} // pdsp namespace end

#endif //PDSP_CONTROL_SLEW_H_INCLUDED
