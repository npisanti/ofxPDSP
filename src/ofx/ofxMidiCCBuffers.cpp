
#include "ofxMidiCCBuffers.h"

#define MIDICCBUFFERSMESSAGERESERVE 32

ofxMidiCCBuffers::ofxMidiCCBuffers(){
    sendClearMessages = true;
  
    setMaxCCNum(9);
    for(int i=0; i<ccSize; ++i){
        ccMessages[i].reserve(MIDICCBUFFERSMESSAGERESERVE);
    }  
}



void ofxMidiCCBuffers::clearAll(){
    sendClearMessages = true;
}

void ofxMidiCCBuffers::setMaxCCNum(int maxCC){
    if(maxCC>0){
        ccSize = maxCC+1;
        ccMessages.resize(ccSize);
        for (int i=0; i<ccSize; i++){
            ccMessages[i].reserve(MIDICCBUFFERSMESSAGERESERVE);
        }
    }
}

void ofxMidiCCBuffers::processMidi (const vector<_ofxPositionedMidiMessage> & readVector, const int &bufferSize ){
    //clear buffers

    for(int i=0; i<ccSize; ++i){
        ccMessages[i].clearMessages();
    }
    
    if(sendClearMessages){
        for(int i=0; i<ccSize; ++i){
            ccMessages[i].addMessage(0.0f, 0);
        }
        sendClearMessages = false;  
    }
    
    for(const _ofxPositionedMidiMessage &midi : readVector){

        if(midi.message.status==MIDI_CONTROL_CHANGE && midi.message.control < ccSize){
            float value = static_cast<float>(midi.message.value +1 )*0.0078125f; 
            if (value==0.0078125f){ value = 0.0f; }
            ccMessages[midi.message.control].addMessage(value, midi.sample); 
        }
    }
    
    for(int i=0; i<ccSize; ++i){
        ccMessages[i].processDestination(bufferSize);
    }
    
}

