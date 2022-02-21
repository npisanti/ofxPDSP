
#include "Wrapper.h"

pdsp::Wrapper::Wrapper() {
    maxChannels = 0;
    buffer = nullptr;
}

void pdsp::Wrapper::setChannels ( int inputs, int outputs ) {
    
    if(inputs<0) inputs = 0;
    if(outputs<0) outputs = 0;
    
    this->inputs.resize( inputs );
    this->outputs.resize( outputs );
    
    if(inputs>outputs){
        maxChannels = inputs;
    }else{
        maxChannels = outputs;
    }

    addOutput( "silent", dummyOut);    
    for ( std::size_t i = 0; i < this->outputs.size(); ++i) {
        addOutput( ofToString(i).c_str(), this->outputs[i] );
    }
            
    for ( std::size_t i = 0; i < this->inputs.size(); ++i) {
        addInput( ofToString(i).c_str(), this->inputs[i] );
    }
            
    updateOutputNodes();
    
}

pdsp::InputNode & pdsp::Wrapper::in( int num ) {
    if( num<0 ) num = 0;
    if( num>=(int)inputs.size() ) num = inputs.size() - 1 ;
    
    return inputs[num];
}

pdsp::OutputNode & pdsp::Wrapper::out( int num ) {
    if( num<0 ) num = 0;
    if( num>=(int)outputs.size() ) num = outputs.size() - 1 ;
    
    return outputs[num];    
}

pdsp::OutputNode & pdsp::Wrapper::out_silent(){
    return dummyOut;
}

void pdsp::Wrapper::process(int bufferSize) noexcept {
    
    ofx_Aeq_Zero( buffer, bufferLength);
    
    for(int i=0; i<(int)inputs.size(); i++){
        
        int inputState;
        const float* inputBuffer = processInput(inputs[i], inputState);
        
        switch(inputState){
            case pdsp::Changed: case pdsp::Unchanged:
                for(int n=0; n<bufferSize; ++n){
                    buffer[n*maxChannels + i] = inputBuffer[0];
                }
            break;
            
            case pdsp::AudioRate:
                for(int n=0; n<bufferSize; ++n){
                    buffer[n*maxChannels + i] = inputBuffer[n];
                }
            break;
        }

    }
    
    audioProcess( buffer, bufferSize, maxChannels );
    
    for(int i=0; i<(int)outputs.size(); i++){
        float* outputBuffer = getOutputBufferToFill( outputs[i] );

        for(int n=0; n<bufferSize; ++n){
            outputBuffer[n] = buffer[n*maxChannels + i];
        }
    }
    
    setOutputToZero(dummyOut);
    
}


void pdsp::Wrapper::prepareUnit( int expectedBufferSize, double sampleRate ) {
    if (buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }
    if( maxChannels == 0){
        std::cout<<"[pdsp] warning, pdsp::Wrapper setup() not executed\n";
        pdsp::pdsp_trace();
    }
    bufferLength = expectedBufferSize*maxChannels;
    ofx_allocate_aligned(buffer, bufferLength);
}


void pdsp::Wrapper::releaseResources()  {
    if (buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }    
}

