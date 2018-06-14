
#include "SequencerBridge.h"

pdsp::Patchable& pdsp::linkBufferToSequencer (MessageBuffer& output, SequencerBridge& input){
        input.link(output);
        input.resetMessageBufferSelector();
        return input;
}


pdsp::Patchable& pdsp::operator>> (pdsp::MessageBuffer& output, pdsp::SequencerBridge& input){
        return linkBufferToSequencer(output, input);
}
