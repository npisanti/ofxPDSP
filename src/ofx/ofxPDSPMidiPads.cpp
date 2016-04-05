
#include "ofxPDSPMidiPads.h"

#define MIDIDRUMKITPROCESSORMESSAGERESERVE 16

ofxPDSPMidiPads::ofxPDSPMidiPads(){
    trigBuffers.clear();
    lowThreshold.clear();
    highThreshold.clear();
    outs_trig.clear();
    size = 0;
}


void ofxPDSPMidiPads::resizeLayers(int size){
    this->size = size;
    trigBuffers.resize(size);
    lowThreshold.resize(size);
    highThreshold.resize(size);
    outs_trig.resize(size);
    for(int i=0; i<size; ++i){
        trigBuffers[i] >> outs_trig[i];
        trigBuffers[i].reserve( MIDIDRUMKITPROCESSORMESSAGERESERVE );
    }
}


void ofxPDSPMidiPads::addTriggerLayer(int noteLow, int noteHigh){
    lowThreshold.push_back(noteLow);
    highThreshold.push_back(noteHigh);
    trigBuffers.resize(size + 1);
    outs_trig.resize(size +1);
    trigBuffers[size] >> outs_trig[size];
    trigBuffers[size].reserve( MIDIDRUMKITPROCESSORMESSAGERESERVE );
    size++;
}


void ofxPDSPMidiPads::setTriggerLayer(int noteLow, int noteHigh, int outIndex){
    if(outIndex>=0 && outIndex<= size){
        lowThreshold[outIndex] = noteLow;
        highThreshold[outIndex] = noteHigh; 
    }
}


void ofxPDSPMidiPads::simpleInit(int lowNote, int numNotes, int span){
    trigBuffers.clear();
    lowThreshold.clear();
    highThreshold.clear();
    outs_trig.clear();
    
    trigBuffers.reserve(numNotes);
    lowThreshold.reserve(numNotes);
    highThreshold.reserve(numNotes);
    outs_trig.reserve(numNotes);  
    
    for(int i=0; i<numNotes; ++i){
        int baseNote = lowNote + i*span;
        addTriggerLayer(baseNote, baseNote+span-1);
    }
    
    size = numNotes;
}


void ofxPDSPMidiPads::processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept{
    
    for(int i=0; i<size; ++i){
        trigBuffers[i].clearMessages();
    }
    
    for(_ofxPositionedMidiMessage &midi : *(midiInProcessor.readVector) ){
        switch(midi.message.status){
            case MIDI_NOTE_ON:
                for(int i=0; i<size; ++i){
                    if(midi.message.pitch >= lowThreshold[i] && midi.message.pitch <= highThreshold[i]){
                        float gateValue = static_cast<float>(midi.message.velocity+1)*0.0078125f; 
                        trigBuffers[i].addMessage(gateValue, midi.sample);
                    }
                }
            break;
            
            case MIDI_NOTE_OFF:
                for(int i=0; i<size; ++i){
                    if(midi.message.pitch >= lowThreshold[i] && midi.message.pitch <= highThreshold[i]){ 
                        trigBuffers[i].addMessage(0.0f, midi.sample);
                    }
                }
            break;
            
            default: break;
        }
    }            
    
    for(int i=0; i<size; ++i){
        trigBuffers[i].processDestination(bufferSize);
    }
    
}
