
// PatchNode.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_PATCHNODE_H_INCLUDED
#define PDSP_CORE_PATCHNODE_H_INCLUDED

#include "BasicNodes.h"
#include <iostream>

namespace pdsp{
    
 
class PatchNode;


//--------------CUSTOM OUTPUTNODE------------------------------------------------
/*!
@cond HIDDEN_SYMBOLS
*/
class PatchOutputNode : public OutputNode{
public:

        void prepareToPlay(int expectedBufferSize, double sampleRate) override {};
        void releaseResources() override {};
    
        void setOversampleLevel(int newOversample) override;
};
/*!
    @endcond
*/

//--------------PATCH NODE------------------------------------------------
/*!
@brief A Unit with no dsp inside, it just pass it's input to the output. Patching float to this Unit is thread-safe.

This is a class can be used as a patch point, it has no dsp inside and can be useful for managing connection to and from a module. You connect to and from it just using >> with no in() and out() methods.
*/  
class PatchNode : public Unit  {
        friend class Processor;
        
public:

    PatchNode();
    PatchNode(const PatchNode& other);
    PatchNode& operator=(const PatchNode& other);
    ~PatchNode();
    
    /*!
    @brief sets the default input value and returns the Unit ready to be patched.
    */  
    Patchable& set(float defaultValue);
    
    /*!
    @brief sets some boundaries to which the values of the buffer will be clamped
    @param[in] lo low boundary (inclusive)
    @param[in] hi high boundary (inclusive)
    */  
    void enableBoundaries( float low, float high);
        
    /*!
    @brief disables the clamping of buffer values
    */  
    void disableBoundaries();
                            
private:

    void process(int bufferSize) noexcept override;
    
    void disableAutomaticProcessing();
    
    int getState();

    const float* getBuffer();
    
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
                      
    InputNode input;
    PatchOutputNode output;
};

//--------------CHANNEL NODE-----------------------------------------
/*!
@brief A module with no dsp inside, just two public pdsp::PatchNode set as default input and output and not connected between then, useful as send/return for coding multichannel modules.

*/  
class ChannelNode : public Patchable {
public:
    ChannelNode();
    PatchNode input;
    PatchNode output;
};

}//END NAMESPACE


#endif  // PATCHNODE_H_INCLUDED
