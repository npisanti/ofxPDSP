
// stockBehaviors.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSPSTOCKBEHAVIORS_H_INCLUDED
#define PDSPSTOCKBEHAVIORS_H_INCLUDED

#include "ScoreCell.h"
#include "../math/random/random.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
struct CellChangeSelf : public pdsp::CellChange{
    int getNextPattern( int currentPattern, int size ) override;
};

struct CellChangeNext : public pdsp::CellChange{
    int getNextPattern( int currentPattern, int size ) override;
};

struct CellChangePrev : public pdsp::CellChange{
    int getNextPattern( int currentPattern, int size ) override;
};

struct CellChangeRandom : public pdsp::CellChange{
    int getNextPattern( int currentPattern, int size ) override;
};

struct CellChangeRandomWalk : public pdsp::CellChange{
    CellChangeRandomWalk(int walk);
    CellChangeRandomWalk();
    
    int getNextPattern( int currentPattern, int size ) override;
    
    int walk;
};

/*!
    @endcond
*/


    /*!
    @brief a collection of ready-made CellChange
    */
class Behavior{

private:
    static CellChangeSelf staticCellChangeSelf;
    static CellChangeNext staticCellChangeNext;
    static CellChangePrev staticCellChangePrev;
    static CellChangeRandom staticCellChangeRandom;
    static CellChangeRandomWalk staticCellChangeRandomWalk;

public:

    /*!
    @brief used by ScoreSection, loops the ScorePattern
    */
    static CellChangeSelf*          Self;

    /*!
    @brief used by ScoreSection, loops the ScorePattern
    */
    static CellChangeSelf*          Loop;
    
    /*!
    @brief used by ScoreSection, plays the next ScorePattern
    */
    static CellChangeNext*          Next;
    /*!
    @brief used by ScoreSection, plays the previous ScorePattern
    */
    static CellChangePrev*          Prev;
    
    /*!
    @brief used by ScoreSection, plays a random ScorePattern
    */
    static CellChangeRandom*        Random;
    
    /*!
    @brief used by ScoreSection, choose a ScorePattern in the next or previous two
    */    
    static CellChangeRandomWalk*    RandomWalk;
};



}//END NAMESPACE

#endif //PDSPSTOCKBEHAVIORS_H_INCLUDED
