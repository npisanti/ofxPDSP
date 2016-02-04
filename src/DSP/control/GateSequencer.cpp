
#include "GateSequencer.h"

//#include <iostream>

pdsp::GateSequencer::GateSequencer(){
        addOutput("trig", output_trig);
        resetOutputToDefault();
        //updateOutputNodes();
        
        singleTrigger = false;
        gateState = false;
        //lastGateValue = pdspGateNull;
        messageBuffer = nullptr;
        
}

void pdsp::GateSequencer::link(MessageBuffer &messageBuffer){
        this->messageBuffer = &messageBuffer;
        messageBuffer.destination = this;
        messageBuffer.connectedToGate = true;
}

void pdsp::GateSequencer::unLink(){
    
        if(messageBuffer!=nullptr){
            messageBuffer->connectedToGate = false;
            messageBuffer->destination = nullptr;
            messageBuffer = nullptr;            
        }
        gateState = false;
        //lastGateValue = pdspGateNull;
}

void pdsp::GateSequencer::setSingleTrigger(bool state){
        singleTrigger = state;
}

pdsp::Patchable& pdsp::GateSequencer::out_trig(){
    return out("trig");
}

void pdsp::GateSequencer::prepareUnit( int expectedBufferSize, double sampleRate ) {}

void pdsp::GateSequencer::releaseResources (){ }

void pdsp::GateSequencer::process (int bufferSize) noexcept {

        if( messageBuffer!=nullptr ){         
            
                //updateBuffer(messageBuffer, bufferSize);
                
                if( ( messageBuffer->empty() ) ){
                        setOutputToZero(output_trig);
                }else{
                        float* trigBuffer = getOutputBufferToFill( output_trig );
                        ofx_Aeq_Zero(trigBuffer, bufferSize); //clean buffer
                        
                        int imax = messageBuffer->size();
                        for( int i=0; i<imax; ++i){
                                ControlMessage &msg = messageBuffer->messages[i];
                                
                                if(msg.value <=0.0f){
                                        trigBuffer[ msg.sample  * getOversampleLevel() ] = pdspTriggerOff;
                                        gateState = false;
                                        //std::cout<<"processing message, trigger off\n";
                                }else{
                                        if(!singleTrigger || (singleTrigger && !gateState) ){
                                                //normal trigger or first trigger with legato
                                                trigBuffer[ msg.sample  * getOversampleLevel() ] =  msg.value;
                                                //std::cout<<"processing message, value"<<msg.value<<"\n";
                                        }else{
                                                //legato triggers
                                                trigBuffer[ msg.sample  * getOversampleLevel() ] = - msg.value;
                                                //std::cout<<"processing message, value"<<msg.value<<"\n";
                                        }
                                        gateState = true;   
                                }
                        }
                }
                
        }else{
                setOutputToZero(output_trig);
        }

}
