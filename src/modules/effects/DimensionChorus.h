
// DimensionChorus.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_DIMENSIONCHORUS_H_INCLUDED
#define PDSP_MODULE_DIMENSIONCHORUS_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/oscillators/phasors/LFOPhasor.h"
#include "../../DSP/oscillators/cheap/CheapTri.h"
#include "../../DSP/delays/Delay.h"
#include "../../DSP/filters/OnePole.h"

namespace pdsp{
/*!
@brief Chorus loosely based on Roland Dimension C-D models.

*/       

class DimensionChorus : public Patchable {

public:
    DimensionChorus();
    DimensionChorus(const DimensionChorus& other);
    DimensionChorus& operator=(const DimensionChorus& other);

    /*!
    @brief Uses the selected channel as input/output for the patching operation. 0 is for the left channel (default input/output) and 1 is for the right channel. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch( std::size_t index );

    /*!
    @brief Sets "speed" as selected input and returns this module ready to be patched. This is the chorus modulation rate in hertz. Init default value is 0.25hz.
    */  
    Patchable& in_speed();
    
    /*!
    @brief Sets "depth" as selected input and returns this module ready to be patched. This is the chorus (bipolar) modulation amount in milliseconds. Init default value is 10ms.
    */  
    Patchable& in_depth();
    
    /*!
    @brief Sets "delay" as selected input and returns this module ready to be patched. This is the chorus delay lines delay. Init default value is 80ms.
    */       
    Patchable& in_delay();
    

    /*!
    @brief returns a value between -1.0f and 1.0f that rapresent the LFO output signal. This method is thread-safe.
    */   
    float meter_lfo() const;


/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("in_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_0();
    
    [[deprecated("in_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_1();
    
    [[deprecated("out_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_0();
    
    [[deprecated("out_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_1();
    
    [[deprecated("in_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_L();
    
    [[deprecated("in_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_R();
    
    [[deprecated("out_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_L();
    
    [[deprecated("out_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_R();
/*!
    @endcond
*/

private:

    struct Channel : public Patchable{
        Channel();
        PatchNode input;
        PatchNode output;
    };

    void patch();
 
    LFOPhasor       phasor;
    CheapTri        LFO;
 
    Amp             mod1;
    Amp             mod2;    
 
    Delay     delay1;
    Delay     delay2;
 
    OnePole         filter1;
    OnePole         filter2;
 
    Channel         channel0;
    Channel         channel1;

    PatchNode       delay;
    PatchNode       depth;
    PatchNode       speed;

};
    
    
    
} // end namespace





#endif // PDSP_MODULE_DIMENSIONCHORUS_H_INCLUDED
