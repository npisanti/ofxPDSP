
// IRVerb.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_IRVERB_H_INCLUDED
#define PDSP_MODULE_IRVERB_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/convolution/FDLConvolver.h"
#include "../../DSP/samplers/SampleBuffer.h"


namespace pdsp{
    
/*!
@brief Impulse Response based reverb, with stereo or mono input and stereo output.
*/       

class IRVerb : public Patchable, public Preparable {


public:

    IRVerb();
    IRVerb(const IRVerb& other);
    IRVerb& operator=(const IRVerb& other);


    /*!
    @brief Sets "mono" as selected input and returns this module ready to be patched. This is the default input. This is the mono reverb input.
    */      
    Patchable& in_mono();

    /*!
    @brief Sets "L" as selected input and returns this module ready to be patched. This is the left reverb input.
    */      
    Patchable& in_L();

    /*!
    @brief Sets "R" as selected input and returns this module ready to be patched. This is the right reverb input.
    */      
    Patchable& in_R();


    /*!
    @brief Sets "L" as selected input and returns this module ready to be patched. This is the left reverb input.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "R" as selected input and returns this module ready to be patched. This is the right reverb input.
    */      
    Patchable& in_1();

        
    /*!
    @brief Sets "L" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "R" as selected output and returns this module ready to be patched. This is the right output channel. 
    */  
    Patchable& out_1();
    
    /*!
    @brief Sets "L" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_L();
    
    /*!
    @brief Sets "R" as selected output and returns this module ready to be patched. This is the right output channel. 
    */  
    Patchable& out_R();

    /*!
    @brief sets the impulse response for the reverb
    @param[in] path path of the impulse response file
    */  
    void loadIR ( std::string path );

    
private:
    void prepareToPlay(int expectedBufferSize, double sampleRate);
    
    void releaseResources();

    void patch();
 
    PatchNode       input_L;
    PatchNode       input_R;
    PatchNode       input_mono;

    FDLConvolver    reverbL;
    FDLConvolver    reverbR;
    
    SampleBuffer    impulse;

    bool monoConnected;
    bool stereoConnected;
    
    void checkMono();
    void checkStereo();

};
    
    
} // end namespace



#endif // PDSP_MODULE_IRVERB_H_INCLUDED
