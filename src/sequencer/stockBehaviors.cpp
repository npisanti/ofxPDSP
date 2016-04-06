
#include "stockBehaviors.h"


int pdsp::CellChangeSelf::getNextPattern( int currentPattern, int size ) {
    return currentPattern;
}


int pdsp::CellChangeNext::getNextPattern( int currentPattern, int size ) {
    currentPattern++;
    if(currentPattern == size){
        return 0;
    }else{
        return currentPattern;
    }
}


int pdsp::CellChangePrev::getNextPattern( int currentPattern, int size ) {
    currentPattern--;
    if(currentPattern == -1){
        return (size-1);
    }else{
        return currentPattern;
    }
}

int pdsp::CellChangeRandom::getNextPattern( int currentPattern, int size ) {
    return dice(size);
}

pdsp::CellChangeRandomWalk::CellChangeRandomWalk(int walk) : walk(walk) {}
pdsp::CellChangeRandomWalk::CellChangeRandomWalk() : walk(2) {}

int pdsp::CellChangeRandomWalk::getNextPattern( int currentPattern, int size ) {
    
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



pdsp::CellChangeSelf pdsp::Behavior::staticCellChangeSelf = pdsp::CellChangeSelf();
pdsp::CellChangeSelf* pdsp::Behavior::Self = &pdsp::Behavior::staticCellChangeSelf;
pdsp::CellChangeSelf* pdsp::Behavior::Loop = &pdsp::Behavior::staticCellChangeSelf;

pdsp::CellChangeNext pdsp::Behavior::staticCellChangeNext = pdsp::CellChangeNext(); 
pdsp::CellChangeNext* pdsp::Behavior::Next = &pdsp::Behavior::staticCellChangeNext;

pdsp::CellChangePrev  pdsp::Behavior::staticCellChangePrev = pdsp::CellChangePrev();
pdsp::CellChangePrev* pdsp::Behavior::Prev = &pdsp::Behavior::staticCellChangePrev;
    
pdsp::CellChangeRandom pdsp::Behavior::staticCellChangeRandom = pdsp::CellChangeRandom();
pdsp::CellChangeRandom* pdsp::Behavior::Random = &pdsp::Behavior::staticCellChangeRandom;

pdsp::CellChangeRandomWalk pdsp::Behavior::staticCellChangeRandomWalk = pdsp::CellChangeRandomWalk();
pdsp::CellChangeRandomWalk* pdsp::Behavior::RandomWalk = &pdsp::Behavior::staticCellChangeRandomWalk;
