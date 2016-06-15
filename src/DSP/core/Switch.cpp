
#include "Switch.h"


pdsp::Switch::Switch(){
    
        output.buffer = nullptr;
        output.state = Unchanged;
        resize(4);
        channel = 0;
        input_select.setDefaultValue(0.0f);
};

pdsp::Switch::Switch(const Switch& other) : Switch(){
    
        output.buffer = other.output.buffer;
        output.state = other.output.state;

        input_select.setDefaultValue(0.0f);
        std::cout<<"[pdsp] warning! switch copy constructed\n";
        pdsp_trace();
};

pdsp::Switch& pdsp::Switch::operator=(const Switch& other){
    
        output.buffer = other.output.buffer;
        return *this;
        std::cout<<"[pdsp] warning! switch node moved\n";
        pdsp_trace();
};


void pdsp::Switch::resize(int size){
    
        this->clearAllAddedNodes();
        
        addInput("select", input_select);
        addOutput("signal", output);

        inputs.resize(size);        
        for(int i=0; i<size; ++i){
            addInput(std::to_string(i).c_str(), inputs[i]);            
        }
        
        updateOutputNodes();
}

pdsp::Switch::~Switch(){
        output.buffer = nullptr; //otherwise output will delete other buffers on decostruction, leading to segfaults
}

pdsp::Patchable& pdsp::Switch::set(int input){
        input_select.setDefaultValue(input);
        return *this;
}

pdsp::Patchable& pdsp::Switch::in_select(){
        return in("select");
}

pdsp::Patchable& pdsp::Switch::in_signal(int channel){
        if( channel < 0 ) channel = 0;
        if( channel > (int) inputs.size()) channel = inputs.size()-1;
        return in( std::to_string(channel).c_str() );
}

pdsp::Patchable& pdsp::Switch::out_signal(){
        return out("signal");
}
             
void pdsp::Switch::prepareUnit( int expectedBufferSize, double sampleRate ) {
        //never prepare or release output, we set output buffer pointer directly
}

void pdsp::Switch::releaseResources() {
        output.buffer = nullptr;
        //never prepare or release output, we set output buffer pointer directly
}
                  
void pdsp::Switch::process(int bufferSize) noexcept {
    
        int state;
        const float * selectBuffer = processInput(input_select, state);
        
        if( state ){ // Changed or AudioRate
            channel = selectBuffer[0];
            if( channel < 0 ) channel = 0;
            if( channel > (int) inputs.size()) channel = inputs.size()-1;
        } 
        
        inputs[channel].process();
        output.state = inputs[channel].state;
        output.buffer = inputs[channel].buffer;
}


