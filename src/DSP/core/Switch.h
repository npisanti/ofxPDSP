
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
@brief A Unit with no dsp inside, it let you switch between different connected inputs and process and pass to the output only the selected input.
*/  

class Switch : public Unit  {
        friend class Processor;
        
public:

    Switch();
    Switch(const Switch& other);
    Switch& operator=(const Switch& other);
    ~Switch();

    /*!
    @brief sets the number of inputs for the switch, you should call this method before patching with input()
    */       
    void resize(int size);
    
    /*!
    @brief Sets "select" as selected input and returns this Unit ready to be patched. This is the default input. This input selects the output from the connected inputs.
    */       
    Patchable& in_select();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the output of the selected input.
    */       
    Patchable& out_signal();

    /*!
    @brief returns an input for connecting one of the outputs to switch. You should call resize() to set the right number of available inputs.
    */     
    InputNode& input( int channel );
    
    /*!
    @brief returns the value of the selected output buffer, updated at control rate. This method is thread safe.
    */ 
    float meter_output() const; 
                            
private:
    int channel;
    
    void process(int bufferSize) noexcept override;
    
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;

    std::atomic<float> meter;
                      
    InputNode input_select;
    std::vector<InputNode> inputs;
    PatchOutputNode output;
    
};

 

}//END NAMESPACE


#endif  // SWITCH_H_INCLUDED
