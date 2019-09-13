
#include "Switch.h"


pdsp::Switch::Switch(){
        addInput("select", input_select);
        addOutput("signal", output);
        updateOutputNodes();
        
        output.buffer = nullptr;
        output.state = Unchanged;
        resize(4);
        channel = 0;
        input_select.setDefaultValue(0.0f);
        meter.store(0.0f);
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
    
        for(auto & input : inputs){
            input.disconnectAll();
        }

        inputs.resize(size);      
        
}

pdsp::Switch::~Switch(){
        output.buffer = nullptr; //otherwise output will delete other buffers on decostruction, leading to segfaults
}


pdsp::Patchable& pdsp::Switch::in_select(){
        return in("select");
}

pdsp::Patchable& pdsp::Switch::out_signal(){
        return out("signal");
}
             
pdsp::InputNode& pdsp::Switch::input(int channel){
        if( channel < 0 ) channel = 0;
        if( channel >= (int) inputs.size()){
            std::cout << "[pdsp] warning! switch input out of range"<< channel <<" set to size "<< (int)inputs.size() << "\n";
            pdsp_trace();
            channel = inputs.size()-1;
        } 
        return inputs[channel];
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
            if( channel >= (int) inputs.size()) channel = inputs.size()-1;
        } 
        
        inputs[channel].process();
        output.state = inputs[channel].state;
        output.buffer = inputs[channel].buffer;
        meter.store( output.buffer[0] );
}

float pdsp::Switch::meter_output() const {
    return meter.load();
}
