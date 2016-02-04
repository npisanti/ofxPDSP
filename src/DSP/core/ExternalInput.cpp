
#include "ExternalInput.h"
#include <iostream>

pdsp::ExternalInput::ExternalInput(){
    output.buffer = nullptr;
    output.state = AudioRate;
    addOutput("signal", output);
    updateOutputNodes();
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}
        
pdsp::Patchable& pdsp::ExternalInput::out_signal(){
    return out("signal");
}
        
        
void pdsp::ExternalInput::copyInput(float* input, const int & bufferSize) noexcept{
    //copy unaligned input
    for(int n=0; n<bufferSize; ++n){
        buffer[n] = input[n];
    }
    inputUpdated = true;
}

void pdsp::ExternalInput::copyInterleavedInput(float* input, int index, int channels, const int & bufferSize) noexcept{
    
    for(int n=0; n<bufferSize; ++n){
        buffer[n] = input[n*channels + index];
    }
    inputUpdated = true;
    
}

void pdsp::ExternalInput::prepareUnit( int expectedBufferSize, double sampleRate ) {
    if(buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }
    ofx_allocate_aligned(buffer, (expectedBufferSize*3)/2);
    output.buffer = buffer;
}

void pdsp::ExternalInput::releaseResources() {
    if(buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }
}

void pdsp::ExternalInput::process(int bufferSize) noexcept {
    
    if(inputUpdated==true){
        output.state = AudioRate;
        inputUpdated = false;
    }else{
        buffer[0] = 0.0f;
        output.state = Changed;
    }
    
}

