
// stockBehaviors.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSPSTOCKBEHAVIORS_H_INCLUDED
#define PDSPSTOCKBEHAVIORS_H_INCLUDED

#include "Sequence.h"
#include "../math/random/random.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
struct SeqChangeSelf : public pdsp::SeqChange{
    int getNextPattern( int currentPattern, int size ) noexcept override;
};

struct SeqChangeNext : public pdsp::SeqChange{
    int getNextPattern( int currentPattern, int size ) noexcept override;
};

struct SeqChangePrev : public pdsp::SeqChange{
    int getNextPattern( int currentPattern, int size ) noexcept override;
};

struct SeqChangeRandom : public pdsp::SeqChange{
    int getNextPattern( int currentPattern, int size ) noexcept override;
};

struct SeqChangeRandomWalk : public pdsp::SeqChange{
    SeqChangeRandomWalk(int walk);
    SeqChangeRandomWalk();
    
    int getNextPattern( int currentPattern, int size ) noexcept override;
    
    int walk;
};

/*!
    @endcond
*/


    /*!
    @brief a collection of ready-made SeqChange
    */
class Behavior{

private:
    static SeqChangeSelf staticSeqChangeSelf;
    static SeqChangeNext staticSeqChangeNext;
    static SeqChangePrev staticSeqChangePrev;
    static SeqChangeRandom staticSeqChangeRandom;
    static SeqChangeRandomWalk staticSeqChangeRandomWalk;

public:

    /*!
    @brief used by ScoreSection, loops the ScorePattern
    */
    static SeqChangeSelf*          Self;

    /*!
    @brief used by ScoreSection, loops the ScorePattern
    */
    static SeqChangeSelf*          Loop;
    
    /*!
    @brief used by ScoreSection, plays the next ScorePattern
    */
    static SeqChangeNext*          Next;
    /*!
    @brief used by ScoreSection, plays the previous ScorePattern
    */
    static SeqChangePrev*          Prev;
    
    /*!
    @brief used by ScoreSection, plays a random ScorePattern
    */
    static SeqChangeRandom*        Random;
    
    /*!
    @brief used by ScoreSection, choose a ScorePattern in the next or previous two
    */    
    static SeqChangeRandomWalk*    RandomWalk;
};



}//END NAMESPACE

#endif //PDSPSTOCKBEHAVIORS_H_INCLUDED
