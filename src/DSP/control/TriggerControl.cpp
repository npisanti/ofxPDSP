
#include "TriggerControl.h"


pdsp::TriggerControl::TriggerControl(){
    
    addOutput("trig", output);
    updateOutputNodes();
    
    nextTrigger.store(0.0f);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}

pdsp::TriggerControl::TriggerControl(const pdsp::TriggerControl & other) {
    std::cout<<"[pdsp] do not copy construct TriggerControls!\n";
    pdsp_trace();    
}


pdsp::Patchable& pdsp::TriggerControl::out_trig(){
    return out("trig");
}


void pdsp::TriggerControl::trigger(float value){
    nextTrigger.store(value);
}

void pdsp::TriggerControl::off(){
    nextTrigger.store(PDSP_TRIGGER_OFF);
}

void pdsp::TriggerControl::prepareUnit( int expectedBufferSize, double sampleRate ){
    
}

void pdsp::TriggerControl::releaseResources () {}



void pdsp::TriggerControl::process (int bufferSize) noexcept {
    
    if(nextTrigger.load() == 0.0f){ 
        
        setOutputToZero(output); 
        
    }else{

        float* outputBuffer = getOutputBufferToFill(output);
        ofx_Aeq_Zero(outputBuffer, bufferSize);
        outputBuffer[0] = nextTrigger.load();
        nextTrigger.store(0.0f);
        
    }


}
