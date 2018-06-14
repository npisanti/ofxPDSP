
#include "MidiPads.h"

#ifndef __ANDROID__

#define MIDIDRUMKITPROCESSORMESSAGERESERVE 16

pdsp::midi::Pads::Pads(){
    trigBuffers.clear();
    lowThreshold.clear();
    highThreshold.clear();
    outs_trig.clear();
    size = 0;
}


void pdsp::midi::Pads::resizeLayers(int size){
    
    int oldSize = this->size;
    this->size = size;
    trigBuffers.resize(size);
    lowThreshold.resize(size);
    highThreshold.resize(size);
    outs_trig.resize(size);
    
    int base;
    if(oldSize==0){
        base = 36;
    }else{
        base = highThreshold[oldSize-1];
    }
    
    for(int i=oldSize; i<size; ++i){
        trigBuffers[i] >> outs_trig[i];
        trigBuffers[i].reserve( MIDIDRUMKITPROCESSORMESSAGERESERVE );
        setTriggerLayer(i, base, base);
        base++;
    }
    
}


void pdsp::midi::Pads::addTriggerLayer(int noteLow, int noteHigh){
    lowThreshold.push_back(noteLow);
    highThreshold.push_back(noteHigh);
    trigBuffers.resize(size + 1);
    outs_trig.resize(size +1);
    trigBuffers[size] >> outs_trig[size];
    trigBuffers[size].reserve( MIDIDRUMKITPROCESSORMESSAGERESERVE );
    size++;
}


void pdsp::midi::Pads::setTriggerLayer(int layerIndex, int noteLow, int noteHigh){
    if(layerIndex>=0 && layerIndex<= size){
        lowThreshold[layerIndex] = noteLow;
        highThreshold[layerIndex] = noteHigh; 
    }
}


void pdsp::midi::Pads::simpleInit(int lowNote, int numLayer, int layerSpan){
    trigBuffers.clear();
    lowThreshold.clear();
    highThreshold.clear();
    outs_trig.clear();
    
    trigBuffers.reserve(numLayer);
    lowThreshold.reserve(numLayer);
    highThreshold.reserve(numLayer);
    outs_trig.reserve(numLayer);  
    
    for(int i=0; i<numLayer; ++i){
        int baseNote = lowNote + i*layerSpan;
        addTriggerLayer(baseNote, baseNote+layerSpan-1);
    }
    
    size = numLayer;
}


int pdsp::midi::Pads::getLayerLow(int layerIndex) const {
    if(layerIndex>=0 && layerIndex<= size){
        return lowThreshold[layerIndex];
    }else{
        return -1;
    }
}


int pdsp::midi::Pads::getLayerHigh(int layerIndex) const {
    if(layerIndex>=0 && layerIndex<= size){
        return highThreshold[layerIndex];
    }else{
        return -1;
    }
}

void pdsp::midi::Pads::processMidi(const pdsp::midi::Input &midiInProcessor, const int &bufferSize ) noexcept{
    
    for(int i=0; i<size; ++i){
        trigBuffers[i].clearMessages();
    }
    
    for( const _PositionedMidiMessage & midi : midiInProcessor.getMessageVector() ){
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

pdsp::SequencerGateOutput &  pdsp::midi::Pads::out_trig( int layerIndex ) {
    return outs_trig[layerIndex];
}

#endif
