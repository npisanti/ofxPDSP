
// Switch.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_SWITCH_H_INCLUDED
#define PDSP_CORE_SWITCH_H_INCLUDED


#include "BasicNodes.h"
#include "PatchNode.h"
#include <iostream>

namespace pdsp{
    

/*!
@brief A Unit with no dsp inside, it just pass one of the inputs to the output. 


*/  
class Switch : public Unit  {
        friend class Processor;
        
public:

    Switch();
    Switch(const Switch& other);
    Switch& operator=(const Switch& other);
    ~Switch();
    
    
    void resize(int size);
    
    
    Patchable& in_select();
    Patchable& out_signal();
    InputNode& input( int channel );

    
                            
private:
    int channel;
    
    void process(int bufferSize) noexcept override;
    
    
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
                      
    InputNode input_select;
    std::vector<InputNode> inputs;
    PatchOutputNode output;
};

 

}//END NAMESPACE


#endif  // SWITCH_H_INCLUDED
