
#include "MessageBuffer.h"


pdsp::MessageBuffer::MessageBuffer(){
    messages.clear();
    destination = nullptr;
    connectedToGate = false;
}


void pdsp::MessageBuffer::clearMessages(){
    messages.clear();
}

void pdsp::MessageBuffer::addMessage(float value, int sample){
    messages.push_back(ControlMessage(value, sample));
}

void pdsp::MessageBuffer::processDestination( const int &bufferSize ){
    if(destination!=nullptr){
        destination->process(bufferSize * destination->getOversampleLevel() );
    }
}

int pdsp::MessageBuffer::size(){
    return messages.size();
}

void pdsp::MessageBuffer::reserve(int size){
    messages.reserve(size);
}

bool pdsp::MessageBuffer::empty(){
    return messages.empty();
}
