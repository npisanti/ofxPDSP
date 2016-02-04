
#include "Sequencer.h"

pdsp::Patchable& pdsp::linkBufferToSequencer (MessageBuffer& output, Sequencer& input){
        input.link(output);
        input.resetMessageBufferSelector();
        return input;
}


pdsp::Patchable& pdsp::operator>> (pdsp::MessageBuffer& output, pdsp::Sequencer& input){
        return linkBufferToSequencer(output, input);
}
