
#include "ScoreCell.h"


pdsp::ScoreMessage::ScoreMessage( double time, float value, int lane) : time(time), value(value), lane(lane) {}

pdsp::ScoreMessage::ScoreMessage( double time, float value ) : time(time), value(value), lane(0) {}

pdsp::ScoreMessage::ScoreMessage() : pdsp::ScoreMessage( 0.0, 0.0f, 0 ) {}

pdsp::ScoreMessage::ScoreMessage(const pdsp::ScoreMessage &other) : pdsp::ScoreMessage( other.time, other.value, other.lane) {}

pdsp::ScoreMessage& pdsp::ScoreMessage::operator= (const pdsp::ScoreMessage &other){
    this->time = other.time;
    this->value = other.value;
    this->lane = other.lane;
    return *this;
}

pdsp::ScoreMessage::~ScoreMessage(){}


bool pdsp::messageSort(const ScoreMessage &lhs, const ScoreMessage &rhs ) { return (lhs.time < rhs.time); }

//---------------------------------------------------------------------------

pdsp::ScoreCell::ScoreCell(){
    score.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    score.clear();
    length = 1.0;
}

void pdsp::ScoreCell::generateScore() noexcept {}

void pdsp::ScoreCell::prepareScore() noexcept {}

void pdsp::ScoreCell::executeGenerateScore(){
    generateScore();
    std::sort (score.begin(), score.end(), messageSort); //sort the messages
}
void pdsp::ScoreCell::executePrepareScore(){
    prepareScore();
}

