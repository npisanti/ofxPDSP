
#include "Amp.h"


pdsp::Amp::Amp(){
        addInput("signal", input_signal);
        addInput("mod", input_mod);
        addOutput("signal", output);
        updateOutputNodes();
    
        input_mod.setDefaultValue(0.0f);
    
        meter.store(0.0f);
        meterOut.store(0.0f);
    
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Amp::~Amp(){
    
}

pdsp::Amp::Amp(const pdsp::Amp & other) : Amp(){
    std::cout<<"[pdsp] warning! amp copy constructed\n"; 
    pdsp_trace();
}
 
pdsp::Amp& pdsp::Amp::operator=(const Amp& other){
    return *this;
    std::cout<<"[pdsp] warning! amp moved constructed\n";
    pdsp_trace();
}


pdsp::Patchable& pdsp::Amp::set(float value){
    input_mod.setDefaultValue(value);
    return *this;
}

float pdsp::Amp::meter_mod() const{
    return meter.load();
}

float pdsp::Amp::meter_output() const{
    return meterOut.load();
}

pdsp::Patchable& pdsp::Amp::in_mod(){
    return in("mod");
}

pdsp::Patchable& pdsp::Amp::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Amp::out_signal(){
    return out("signal");
}


void pdsp::Amp::enableBoundaries(float low, float high){
    input_mod.enableBoundaries(low, high);
}


void pdsp::Amp::disableBoundaries(){
    input_mod.disableBoundaries();
}
    

void pdsp::Amp::prepareUnit ( int expectedBufferSize, double sampleRate ) {

}

void pdsp::Amp::releaseResources () {

}

void pdsp::Amp::process (int bufferSize) noexcept {
    
    int modState;
    const float* modBuffer = processInput(input_mod, modState);
    meter.store(modBuffer[0]);
        
    if ( modBuffer[0] == 0.0f && modState != AudioRate ){

        setOutputToZero(output);
        meterOut.store(0.0f);   
             
    }else{        
           
        int signalState;
        const float* signalBuffer = processInput(input_signal, signalState);

        int switcher = signalState + modState*4;

                //process audio rate
        switch ( switcher & 42 ) {
        case 0:  // signal control rate, mod control rate
            {
                float out = modBuffer[0]*signalBuffer[0];
                setControlRateOutput(output, out); 
                meterOut.store(out);  
            }
                break;
        case 2:  // signal audio rate, mod control rate
            {
                float* outputBuffer = getOutputBufferToFill(output);
                ofx_Aeq_BmulS(outputBuffer, signalBuffer, modBuffer[0], bufferSize);
                meterOut.store(outputBuffer[0]);  
            }
                break;
        case 8:  // signal control rate, mod audio rate
            {
                float* outputBuffer = getOutputBufferToFill(output);
                ofx_Aeq_BmulS(outputBuffer, modBuffer, signalBuffer[0], bufferSize);
                meterOut.store(outputBuffer[0]);  
            }
                break;
        case 10: // signal audio rate, mod audio rate   
            {
                float* outputBuffer = getOutputBufferToFill(output);
                ofx_Aeq_BmulC(outputBuffer, signalBuffer, modBuffer, bufferSize);
                meterOut.store(outputBuffer[0]);  
            }
        default: break;
        }

        
        
    }
    
}
