
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
@brief Two FDLConvolver in one module

*/    

class StereoConvolver : public Patchable {

public:
    StereoConvolver();
    StereoConvolver(const StereoConvolver& other);
    StereoConvolver& operator=(const StereoConvolver& other);

    //StereoConvolver(StereoConvolver&& other);
    //StereoConvolver& operator=( StereoConvolver&& other);
    //StereoConvolver& operator=( StereoConvolver&& other);

    /*!
    @brief loads the given impulse response to both the stereo channels.
    */       
    void loadIR ( SampleBuffer* impulseResponse);
    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono convolver.
    */  
    Patchable& out_1();
    
private:
    void patch();
 
    FDLConvolver    channel1;
    FDLConvolver    channel2;
 
 
};
    
    
    
} // end namespace


#endif // PDSP_MODULE_STEREOCONVOLVER_H_INCLUDED
