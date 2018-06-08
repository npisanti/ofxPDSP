
#include "ValueSequencer.h"


pdsp::ValueSequencer::ValueSequencer(){
        
    addOutput("signal", output);
    resetOutputToDefault();

    setSlewTime(0.0f);
    
    connectToSlewControl = false;

    messageBuffer = nullptr;
    slewControl = nullptr;

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::ValueSequencer::ValueSequencer(const ValueSequencer & other){
    std::cout<<"[pdsp] do not copy construct ValueSequencers!\n";    
    pdsp_trace();
}

pdsp::Patchable& pdsp::ValueSequencer::out_signal(){
    return out("signal");   
}

float pdsp::ValueSequencer::meter_output() const{
    return slewLastValue.load();
}

void pdsp::ValueSequencer::link(MessageBuffer &messageBuffer){
    if(connectToSlewControl){
        this->slewControl = &messageBuffer;  
    }else{
        this->messageBuffer = &messageBuffer;
        messageBuffer.destination = this;   
    }
}

void pdsp::ValueSequencer::resetMessageBufferSelector() {
        connectToSlewControl = false;
}

pdsp::Sequencer& pdsp::ValueSequencer::in_message(){
        connectToSlewControl = false;
        return *this;
}

pdsp::Sequencer& pdsp::ValueSequencer::in_slew(){
        connectToSlewControl = true;
        return *this;
}

void pdsp::ValueSequencer::enableSmoothing(float timeMs){
    this->setSlewTime(timeMs);
    
}

void pdsp::ValueSequencer::disableSmoothing(){
    this->deactivateSlew();
}

void pdsp::ValueSequencer::unLink(){
    if(messageBuffer != nullptr){
        messageBuffer->destination = nullptr;
        messageBuffer = nullptr;
    }
}

void pdsp::ValueSequencer::prepareUnit( int expectedBufferSize, double sampleRate ) {
        this->sampleRate = sampleRate;
}


void pdsp::ValueSequencer::releaseResources (){ }




void pdsp::ValueSequencer::process (int bufferSize) noexcept {
        
        //if(messageBuffer!=nullptr){
               
        if( messageBuffer->empty() ){

                if(slewRun){
                        float* outputBuffer = getOutputBufferToFill(output);
                        runSlewBlock( outputBuffer, 0, bufferSize);
                }else{
                        setControlRateOutput(output, slewLastValue);
                }
                
        } else {

                float* outputBuffer = getOutputBufferToFill(output);
                
                int n=0;
                int k=0;
                int imax = messageBuffer->size();
                for( int i=0; i<imax; ++i){
                        ControlMessage &msg = messageBuffer->messages[i];

                        if(slewRun){
                                 runSlewBlock(outputBuffer, n, msg.sample * getOversampleLevel());
                        }else{
                                 ofx_Aeq_S_range(outputBuffer, slewLastValue, n, msg.sample * getOversampleLevel());
                        }
                        
                        if( slewControl != nullptr && 
                           (k < slewControl->messages.size()) && 
                           (slewControl->messages[k].sample <= msg.sample))
                        {
                                slewTimeMod = slewControl->messages[k].value;
                                k++; 
                        }
                        
                        valueChange(msg.value);
                        
                        n = msg.sample * getOversampleLevel();
                        
                }

                if(slewRun){
                         runSlewBlock(outputBuffer, n, bufferSize );
                }else{
                         ofx_Aeq_S_range(outputBuffer, slewLastValue, n, bufferSize);
                }               

        }
        //}else{
        //        setControlRateOutput(output, slewLastValue);
        //}
        
        if(slewControl != nullptr && !slewControl->empty() ){ //we set the slew control with the last of the buffer
                slewTimeMod = slewControl->messages[ slewControl->messages.size() - 1 ].value;
        }

}
