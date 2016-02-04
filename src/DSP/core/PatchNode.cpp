
#include "PatchNode.h"


void pdsp::PatchOutputNode::setOversampleLevel(int newOversample) {
    oversampleLevel = newOversample;
}


pdsp::PatchNode::PatchNode(){
        output.buffer = nullptr;
        output.state = Unchanged;
        addInput("patch", input);
        addOutput("patch", output);
        updateOutputNodes();
        input.setDefaultValue(0.0f);
        
};

pdsp::PatchNode::PatchNode(const PatchNode& other) : PatchNode(){
        output.buffer = other.output.buffer;
        output.state = other.output.state;

        input.setDefaultValue(0.0f);
        std::cout<<"patch node copy constructed\n";
};

pdsp::PatchNode& pdsp::PatchNode::operator=(const PatchNode& other){
        output.buffer = other.output.buffer;
        return *this;
        std::cout<<"patch node moved\n";
};


pdsp::PatchNode::~PatchNode(){
        output.buffer = nullptr; //otherwise output will delete other buffers on decostruction, leading to segfaults
}


/*
pdsp::OutputNode& pdsp::PatchNode::getDefaultOutput(){
        return output;
}

pdsp::InputNode& pdsp::PatchNode::getDefaultInput(){
        return input;
}
*/

pdsp::Patchable& pdsp::PatchNode::set(float defaultValue){
    input.setDefaultValue(defaultValue);
    return *this;
}


void pdsp::PatchNode::enableBoundaries( float low, float high){
        input.enableBoundaries(low, high);
}
                    
void pdsp::PatchNode::disableBoundaries(){
        input.disableBoundaries();
}
                    
void pdsp::PatchNode::prepareUnit( int expectedBufferSize, double sampleRate ) {
        //never prepare or release output, we set output buffer pointer directly
}

void pdsp::PatchNode::releaseResources() {
        //never prepare or release output, we set output buffer pointer directly
}

/*
void pdsp::PatchNode::disconnectAll(){
        input.disconnectAll();
        output.disconnectAll();
}
*/
                  
void pdsp::PatchNode::process(int bufferSize) noexcept {
        input.process();
        output.state = input.state;
        output.buffer = input.buffer;
}

void pdsp::PatchNode::disableAutomaticProcessing(){
        output.parent = nullptr;
}

int pdsp::PatchNode::getState(){
        return output.state;
}

const float* pdsp::PatchNode::getBuffer(){
        return output.buffer;
}

