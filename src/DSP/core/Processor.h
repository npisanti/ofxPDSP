
// Processor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CORE_PROCESSOR_H_INCLUDED
#define PDSP_CORE_PROCESSOR_H_INCLUDED

#include "BasicNodes.h"
#include "PatchNode.h"
#include <vector>

namespace pdsp{
    
    /*!
    @brief The bridge between pdsp and the audio callback
    
    One of the processAndCopy... method of this class has to be called inside the audio callback, it will recursively process all the Units and modules patched to the input channels and copy the results to the audio callback. The standard constructor has 2 channels, but you can change the number of channels simply using resize() on the channels vector.
    */   

class Processor {
public:

    Processor( int channels );
    Processor();
        
    /*!
    @brief simply process the channes without copying them, useful if you want just to split the processing in multiple calls.

    Sometimes maybe you want to split the process in multiple process calls, so you can patch a Processor channels into another processor, call process() on the first processor and processAndCopy on the second. 
    */  
    void process(const int &bufferSize) noexcept;
        
    /*!
    @brief process all the DSP recursively and copies them to an array of pointer to arrays (channels)
    @param[in] bufferToFill multidimensional array of audio buffers to render
    @param[in] channelsNum number of channels
    @param[in] bufferSize number of samples to render
    */  
    void processAndCopyOutput(float** bufferToFill, const int &channelsNum, const int &bufferSize) noexcept;        
    
    /*!
    @brief process all the DSP recursively and copies the result to an array of interleaved values
    @param[in] bufferToFill interleaved array of audio buffers to render
    @param[in] channelsNum number of channels
    @param[in] bufferSize number of samples to render
    */  
    void processAndCopyInterleaved(float* bufferToFill, const int &channelsNum, const int &bufferSize) noexcept;  
    
    /*!
    @brief an array of channels. Patch your Units and modules to this channels thinking to them as your system final output
    */  
    std::vector<PatchNode> channels;
   
    /*!
    @brief change the number of output channels;
    @param[in] channelsNum number of channels
    */  
    void resize( int channelsNum );
    
    /*!
    @brief all the connectio patched to this will be processed but not outputted. Patch your Units and modules to this channels if you need them to be always active for some reason;
    */  
    PatchNode blackhole;
};
        
        
}



#endif  // PROCESSOR_H_INCLUDED
