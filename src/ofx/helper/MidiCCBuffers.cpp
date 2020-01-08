
#include "MidiCCBuffers.h"

#ifndef __ANDROID__

#define MIDICCBUFFERSMESSAGERESERVE 32

namespace pdsp{ namespace helper{

MidiCCBuffers::MidiCCBuffers(){
    sendClearMessages = true;
}

void MidiCCBuffers::clearAll(){
    sendClearMessages = true;
}

void MidiCCBuffers::setMaxCCNum(int maxCC){
}

pdsp::MessageBuffer & MidiCCBuffers::out(int cc) {
    if (ccMessages.find(cc) == ccMessages.end()) {
        ccMessages[cc].reserve(MIDICCBUFFERSMESSAGERESERVE);
    }
    return ccMessages[cc];
}

int MidiCCBuffers::getCCSize() {
    return ccMessages.size();
}

void MidiCCBuffers::processMidi (const std::vector<_PositionedMidiMessage> & readVector, const int &bufferSize ){
    //clear buffers

    for (auto& cc : ccMessages) {
        cc.second.clearMessages();
    }

    if(sendClearMessages){
        for (auto& cc : ccMessages) {
            cc.second.addMessage(0.0f, 0);
        }
        sendClearMessages = false;
    }

    for(const _PositionedMidiMessage &midi : readVector){
        if (midi.message.status == MIDI_CONTROL_CHANGE && ccMessages.find(midi.message.control) != ccMessages.end()) {
            float value = static_cast<float>(midi.message.value + 1) * 0.0078125f;
            if (value == 0.0078125f) { value = 0.0f; }
            ccMessages[midi.message.control].addMessage(value, midi.sample);
        }
    }

    for (auto& cc : ccMessages) {
        cc.second.processDestination(bufferSize);
    }

}

}} // end namespace

#endif
