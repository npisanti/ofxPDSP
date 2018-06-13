
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

    //DimensionChorus(DimensionChorus&& other);
    //DimensionChorus& operator=( DimensionChorus&& other);
    //DimensionChorus& operator=( DimensionChorus&& other);


    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();

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
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono chorus.
    */  
    Patchable& out_1();

    /*!
    @brief returns a value between -1.0f and 1.0f that rapresent the LFO output signal. This method is thread-safe.
    */   
    float meter_lfo() const;
    
private:
    void patch();
 
    LFOPhasor       phasor;
    CheapTri        LFO;
 
    Amp             mod1;
    Amp             mod2;    
 
    Delay     delay1;
    Delay     delay2;
 
    OnePole         filter1;
    OnePole         filter2;
 
    PatchNode       input1;
    PatchNode       input2;
    PatchNode       output1;
    PatchNode       output2;

    PatchNode       delay;
    PatchNode       depth;
    PatchNode       speed;

};
    
    
    
} // end namespace





#endif // PDSP_MODULE_DIMENSIONCHORUS_H_INCLUDED
