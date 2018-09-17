
// LinearCrossfader.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_LINEARCROSSFADER_H_INCLUDED
#define PDSP_MODULE_LINEARCROSSFADER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/utility/OneMinusInput.h"

namespace pdsp{
    /*!
    @brief Linearly fades between A/B inputs.
    */  
class LinearCrossfader : public Patchable {

public:
    LinearCrossfader();
    LinearCrossfader(const LinearCrossfader& other);
    LinearCrossfader& operator=(const LinearCrossfader& other);

    
    /*!
    @brief meters the fader position between 0.0f and 1.0f. This method is thread safe.
    */  
    float meter_fade();
    
    /*!
    @brief Sets "A" as selected input and returns this module ready to be patched. This is the default input. This is the channel at 0.0f position.
    */      
    Patchable& in_A();

    /*!
    @brief Sets "B" as selected input and returns this module ready to be patched. This is the channel at 1.0f fade position.
    */     
    Patchable& in_B();
    
    /*!
    @brief Sets "fade" as selected input and returns this module ready to be patched. This input is clampled to [0.0f 1.0f] range, at 0.0f only the first signal pass, at 1.0f only the second signal pass, the signal are crossfaded for all the values in between. At 0.0f and 1.0f the muted signal is not even processed.
    */         
    Patchable& in_fade();    

    
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the crossfader output.
    */ 
    Patchable& out_signal();  


/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("in_0() deprecated, use in_A() instead")]]
    Patchable& in_0();
    
    [[deprecated("in_1() deprecated, use in_B() instead")]]
    Patchable& in_1();
/*!
    @endcond
*/
    
private:
    void patch();
 
    Amp             in1amp;
    Amp             in2amp;
    OneMinusInput   oneMinusX;
    PatchNode       control;
    PatchNode       output;

};
    
    
    
} // end namespace

#endif // PDSP_MODULE_LINEARCROSSFADER_H_INCLUDED
