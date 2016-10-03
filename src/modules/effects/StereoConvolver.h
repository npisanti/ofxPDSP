
// StereoConvolver.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_STEREOCONVOLVER_H_INCLUDED
#define PDSP_MODULE_STEREOCONVOLVER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/convolution/FDLConvolver.h"
#include "../../DSP/samplers/SampleBuffer.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/ 

class [[deprecated("class deprecated, replaced by IRVerb")]] StereoConvolver : public Patchable {

public:
    StereoConvolver();
    StereoConvolver(const StereoConvolver& other);
    StereoConvolver& operator=(const StereoConvolver& other);
      
    void loadIR ( SampleBuffer* impulseResponse);
     
    Patchable& in_0();
   
    Patchable& in_1();

    Patchable& out_0();
    
    Patchable& out_1();
    
private:
    void patch();
 
    FDLConvolver    channel1;
    FDLConvolver    channel2;
 
 
};
    
/*!
    @endcond
*/  
    
} // end namespace


#endif // PDSP_MODULE_STEREOCONVOLVER_H_INCLUDED
