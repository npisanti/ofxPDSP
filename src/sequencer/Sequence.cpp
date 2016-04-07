
#include "Sequence.h"

pdsp::Sequence::Sequence( double stepDivision ){ 
    modified = false;    
    setDivision(stepDivision);
    nextScore.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    code = []() noexcept {};
}

pdsp::Sequence::Sequence() : Sequence (16.0){}

void pdsp::Sequence::setDivision( double value ){
    this->div = value;
    divMult = 1.0 / div;
}

void pdsp::Sequence::setLength( double value ){
    this->length = value;
}


pdsp::Sequence::Sequence(const Sequence & other) {
    this->modified = other.modified.load();
    setDivision(other.div);
    score = other.score;
    nextScore.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    nextScore = other.nextScore;
    code = other.code;
}

pdsp::Sequence::Sequence(Sequence && other) {
    this->modified = other.modified.load();
    setDivision(other.div);
    score = other.score;
    nextScore.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    nextScore = other.nextScore;
    code = other.code;
}

pdsp::Sequence& pdsp::Sequence::operator= (const Sequence & other) {
    this->modified = other.modified.load();
    setDivision(other.div);
    score = other.score;
    nextScore.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    nextScore = other.nextScore;
    code = other.code;
    return *this;
}

pdsp::Sequence& pdsp::Sequence::operator= (Sequence && other) {
    this->modified = other.modified.load();
    setDivision(other.div);
    score = other.score;
    nextScore.reserve(PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT);
    nextScore = other.nextScore;
    code = other.code;
    return *this;
}


void pdsp::Sequence::set( std::initializer_list<float> init, int outputs ) noexcept{
    if(modified==true){
        std::cout<<"[pdsp] warning! you have already set this Sequence, but it hasn't been processed yet, please set it once and wait for the changes to be effective before setting it again to avoid race conditions!\n";
        pdsp_trace();
    }
    
    double time=0.0;
    int out = 0;
    for (auto &value : init){
        if( value >= 0.0f){
            nextScore.push_back(  pdsp::ScoreMessage( time , value, out) );            
        }
        
        out++;
        if( out == outputs ){
            time += divMult;
            out = 0;
        }
    }
    modified = true;
}

void pdsp::Sequence::set( std::initializer_list<float> init, double division, double length, int outputs ) noexcept{
    setDivision(division);
    setLength(length);
    set(init, outputs);
}

void pdsp::Sequence::set( const std::vector<float> &init, int outputs ) noexcept{
    if(modified==true){
        std::cout<<"[pdsp] warning! you have already set this Sequence, but it hasn't been processed yet, please set it once and wait for the changes to be effective before setting it again to avoid race conditions!\n";
        pdsp_trace();
    }
    
    double time=0.0;
    int out = 0;
    for (auto &value : init){
        if( value >= 0.0f){
            nextScore.push_back(  pdsp::ScoreMessage( time , value, out) );            
        }
        
        out++;
        if( out == outputs ){
            time += divMult;
            out = 0;
        }
    }
    modified = true;
}

void pdsp::Sequence::set( const std::vector<float> &init, double division, double length, int outputs ) noexcept{
    setDivision(division);
    setLength(length);
    set(init, outputs);
}

void pdsp::Sequence::message(double step, float value, int outputIndex) noexcept{
    nextScore.push_back( pdsp::ScoreMessage( step * divMult, value, outputIndex) );
}

void pdsp::Sequence::begin() noexcept{
    nextScore.clear();
}

void pdsp::Sequence::end() noexcept{
    modified = true;
}


void pdsp::Sequence::generateScore() noexcept {
    code();
    if(modified){
        score.swap( nextScore ); // swap score in a thread-safe section
        modified = false;
    }
}

pdsp::SeqChange::SeqChange(){
    code = [&]() noexcept { return self; };
}

int pdsp::SeqChange::getNextPattern( int currentPattern, int size ) noexcept {
    self = currentPattern;
    return code();
}
