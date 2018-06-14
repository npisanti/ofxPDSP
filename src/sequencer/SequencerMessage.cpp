
#include "SequencerMessage.h"


pdsp::SequencerMessage::SequencerMessage( double time, float value, int lane) : time(time), value(value), lane(lane) {}

pdsp::SequencerMessage::SequencerMessage( double time, float value ) : time(time), value(value), lane(0) {}

pdsp::SequencerMessage::SequencerMessage() : pdsp::SequencerMessage( 0.0, 0.0f, 0 ) {}

pdsp::SequencerMessage::SequencerMessage(const pdsp::SequencerMessage &other) : pdsp::SequencerMessage( other.time, other.value, other.lane) {}

pdsp::SequencerMessage& pdsp::SequencerMessage::operator= (const pdsp::SequencerMessage &other){
    this->time = other.time;
    this->value = other.value;
    this->lane = other.lane;
    return *this;
}

pdsp::SequencerMessage::~SequencerMessage(){}


bool pdsp::messageSort(const SequencerMessage &lhs, const SequencerMessage &rhs ) { return (lhs.time < rhs.time); }
