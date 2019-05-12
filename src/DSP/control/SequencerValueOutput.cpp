
#include "SequencerValueOutput.h"


pdsp::SequencerValueOutput::SequencerValueOutput(){
        
    addOutput("signal", output);
    resetOutputToDefault();

    setSlewTime(0.0f);
    
    connectToSlewControl = false;

    messageBuffer = nullptr;
    slewControl = nullptr;

    firstMessage = true;
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}

pdsp::SequencerValueOutput::SequencerValueOutput(const SequencerValueOutput & other){
    std::cout<<"[pdsp] do not copy construct SequencerValueOutputs!\n";    
    pdsp_trace();
}

pdsp::Patchable& pdsp::SequencerValueOutput::out_signal(){
    return out("signal");   
}

float pdsp::SequencerValueOutput::meter_output() const{
    return slewLastValue.load();
}

void pdsp::SequencerValueOutput::link(MessageBuffer &messageBuffer){
    if(connectToSlewControl){
        this->slewControl = &messageBuffer;  
    }else{
        this->messageBuffer = &messageBuffer;
        messageBuffer.destination = this;   
    }
}

void pdsp::SequencerValueOutput::resetMessageBufferSelector() {
        connectToSlewControl = false;
}

pdsp::SequencerBridge& pdsp::SequencerValueOutput::in_message(){
        connectToSlewControl = false;
        return *this;
}

pdsp::SequencerBridge& pdsp::SequencerValueOutput::in_slew(){
        connectToSlewControl = true;
        return *this;
}

void pdsp::SequencerValueOutput::enableSmoothing(float timeMs){
    this->setSlewTime(timeMs);
    
}

void pdsp::SequencerValueOutput::disableSmoothing(){
    this->deactivateSlew();
}

void pdsp::SequencerValueOutput::unLink(){
    if(messageBuffer != nullptr){
        messageBuffer->destination = nullptr;
        messageBuffer = nullptr;
    }
}

void pdsp::SequencerValueOutput::prepareUnit( int expectedBufferSize, double sampleRate ) {
        this->sampleRate = sampleRate;
}


void pdsp::SequencerValueOutput::releaseResources (){ }




void pdsp::SequencerValueOutput::process (int bufferSize) noexcept {
        
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

                        if(firstMessage){
                            slewRun = false;
                            slewLastValue = msg.value;
                            firstMessage = false;
                        }

                        if(slewRun){
                                 runSlewBlock(outputBuffer, n, msg.sample * getOversampleLevel());
                        }else{
                                 ofx_Aeq_S_range(outputBuffer, slewLastValue, n, msg.sample * getOversampleLevel());
                        }
                        
                        if( slewControl != nullptr && 
                           (k < int(slewControl->messages.size())) && 
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

void pdsp::SequencerValueOutput::resetSmoothing(){
    firstMessage = true;
}
