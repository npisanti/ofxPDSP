
#include "ofxPDSPAddonBridge.h"

ofxPDSPAddonBridge::ofxPDSPAddonBridge() {
    
    maxChannels = 0;
    buffer = nullptr;

}

void ofxPDSPAddonBridge::setChannels ( int inputs, int outputs ) {
    
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
    for ( size_t i = 0; i < this->outputs.size(); ++i) {
        addOutput( std::to_string(i).c_str(), this->outputs[i] );
    }
            
    for ( size_t i = 0; i < this->inputs.size(); ++i) {
        addInput( std::to_string(i).c_str(), this->inputs[i] );
    }
            
    updateOutputNodes();
    
}

pdsp::InputNode & ofxPDSPAddonBridge::in( int num ) {
    if( num<0 ) num = 0;
    if( num>=(int)inputs.size() ) num = inputs.size() - 1 ;
    
    return inputs[num];
}

pdsp::OutputNode & ofxPDSPAddonBridge::out( int num ) {
    if( num<0 ) num = 0;
    if( num>=(int)outputs.size() ) num = outputs.size() - 1 ;
    
    return outputs[num];    
}

pdsp::OutputNode & ofxPDSPAddonBridge::out_silent(){
    return dummyOut;
}

void ofxPDSPAddonBridge::process(int bufferSize) noexcept {
    
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


void ofxPDSPAddonBridge::prepareUnit( int expectedBufferSize, double sampleRate ) {
    if (buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }
    if( maxChannels == 0){
        std::cout<<"[pdsp] warning, ofxPDSPAddonBridge setup() not executed\n";
        pdsp::pdsp_trace();
    }
    bufferLength = expectedBufferSize*maxChannels;
    ofx_allocate_aligned(buffer, bufferLength);
}


void ofxPDSPAddonBridge::releaseResources()  {
    if (buffer != nullptr){
        ofx_deallocate_aligned(buffer);
    }    
}

