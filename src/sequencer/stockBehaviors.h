
// stockBehaviors.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSPSTOCKBEHAVIORS_H_INCLUDED
#define PDSPSTOCKBEHAVIORS_H_INCLUDED

#include "Sequence.h"
#include "../math/random/random.h"

/*!
    @cond HIDDEN_SYMBOLS
*/

namespace pdsp{

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

struct SeqChangeRandomOther : public pdsp::SeqChange{
    int getNextPattern( int currentPattern, int size ) noexcept override;
};

struct SeqChangeRandomWalk : public pdsp::SeqChange{
    SeqChangeRandomWalk(int walk);
    SeqChangeRandomWalk();
    
    int getNextPattern( int currentPattern, int size ) noexcept override;
    
    int walk;
};




    /*!
    @brief a collection of ready-made SeqChange
    */
class Behavior{

private:
    static SeqChangeSelf staticSeqChangeSelf;
    static SeqChangeNext staticSeqChangeNext;
    static SeqChangePrev staticSeqChangePrev;
    static SeqChangeRandom staticSeqChangeRandom;
    static SeqChangeRandomOther staticSeqChangeRandomOther;
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
    @brief used by ScoreSection, plays a random ScorePattern different from the last played
    */
    static SeqChangeRandomOther*   RandomOther;
    
    /*!
    @brief used by ScoreSection, choose a ScorePattern in the next or previous two
    */    
    static SeqChangeRandomWalk*    RandomWalk;

    /*!
    @brief used by ScoreSection, nothing will play next, nullptr is the same
    */        
    static SeqChange*              Nothing;

    /*!
    @brief used by ScoreSection, nothing will play next, nullptr is the same
    */        
    static SeqChange*              Stop;
    
    /*!
    @brief used by ScoreSection, nothing will play next, nullptr is the same
    */        
    static SeqChange*              OneShot;
};

}//END NAMESPACE

/*!
    @endcond
*/

#endif //PDSPSTOCKBEHAVIORS_H_INCLUDED
