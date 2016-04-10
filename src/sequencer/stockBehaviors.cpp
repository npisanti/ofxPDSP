
#include "stockBehaviors.h"


int pdsp::SeqChangeSelf::getNextPattern( int currentPattern, int size ) noexcept {
    return currentPattern;
}


int pdsp::SeqChangeNext::getNextPattern( int currentPattern, int size ) noexcept {
    currentPattern++;
    if(currentPattern == size){
        return 0;
    }else{
        return currentPattern;
    }
}


int pdsp::SeqChangePrev::getNextPattern( int currentPattern, int size ) noexcept {
    currentPattern--;
    if(currentPattern == -1){
        return (size-1);
    }else{
        return currentPattern;
    }
}

int pdsp::SeqChangeRandom::getNextPattern( int currentPattern, int size ) noexcept {
    return dice(size);
}

int pdsp::SeqChangeRandomOther::getNextPattern( int currentPattern, int size ) noexcept {
    int toReturn = currentPattern + 1 + dice(size-1);
    while (toReturn>=size) { toReturn -= size; }
    return toReturn;
}

pdsp::SeqChangeRandomWalk::SeqChangeRandomWalk(int walk) : walk(walk) {}
pdsp::SeqChangeRandomWalk::SeqChangeRandomWalk() : walk(2) {}

int pdsp::SeqChangeRandomWalk::getNextPattern( int currentPattern, int size ) noexcept {
    
    int step = randomBipolarInt(-walk, walk);
    currentPattern += step;
    while(currentPattern < 0 ){
        currentPattern = size + currentPattern;
    }
    while(currentPattern >= size ){
        currentPattern -= size;
    }
    return currentPattern;
}



pdsp::SeqChangeSelf pdsp::Behavior::staticSeqChangeSelf = pdsp::SeqChangeSelf();
pdsp::SeqChangeSelf* pdsp::Behavior::Self = &pdsp::Behavior::staticSeqChangeSelf;
pdsp::SeqChangeSelf* pdsp::Behavior::Loop = &pdsp::Behavior::staticSeqChangeSelf;

pdsp::SeqChangeNext pdsp::Behavior::staticSeqChangeNext = pdsp::SeqChangeNext(); 
pdsp::SeqChangeNext* pdsp::Behavior::Next = &pdsp::Behavior::staticSeqChangeNext;

pdsp::SeqChangePrev  pdsp::Behavior::staticSeqChangePrev = pdsp::SeqChangePrev();
pdsp::SeqChangePrev* pdsp::Behavior::Prev = &pdsp::Behavior::staticSeqChangePrev;
    
pdsp::SeqChangeRandom pdsp::Behavior::staticSeqChangeRandom = pdsp::SeqChangeRandom();
pdsp::SeqChangeRandom* pdsp::Behavior::Random = &pdsp::Behavior::staticSeqChangeRandom;
    
pdsp::SeqChangeRandomOther pdsp::Behavior::staticSeqChangeRandomOther = pdsp::SeqChangeRandomOther();
pdsp::SeqChangeRandomOther* pdsp::Behavior::RandomOther = &pdsp::Behavior::staticSeqChangeRandomOther;

pdsp::SeqChangeRandomWalk pdsp::Behavior::staticSeqChangeRandomWalk = pdsp::SeqChangeRandomWalk();
pdsp::SeqChangeRandomWalk* pdsp::Behavior::RandomWalk = &pdsp::Behavior::staticSeqChangeRandomWalk;

pdsp::SeqChange* pdsp::Behavior::Nothing = nullptr;

