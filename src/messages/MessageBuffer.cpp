
#include "MessageBuffer.h"
#include <iostream>

pdsp::MessageBuffer::MessageBuffer(){
    messages.clear();
    destination = nullptr;
    connectedToGate = false;
    reserve(PDSP_SCORESECTIONMESSAGERESERVE);
}

pdsp::MessageBuffer::MessageBuffer(const MessageBuffer & other){
    this->messages.clear();
    this->destination = other.destination;
    if (this->destination != nullptr ){
        this->destination->messageBuffer = this;
    }
    this->connectedToGate = other.connectedToGate;
    this->reserve(PDSP_SCORESECTIONMESSAGERESERVE);
    //std::cout << "message buffer copy constructed\n";
}

pdsp::MessageBuffer& pdsp::MessageBuffer::operator=(const MessageBuffer & other){
    this->messages.clear();
    this->destination = other.destination;
    if (this->destination != nullptr ){
        this->destination->messageBuffer = this;
    }
    this->connectedToGate = other.connectedToGate;
    this->reserve(PDSP_SCORESECTIONMESSAGERESERVE);
    //std::cout << "message buffer moved\n";
    return *this;
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
