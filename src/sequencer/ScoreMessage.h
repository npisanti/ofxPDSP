
// ScoreMessage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SCOREMESSAGE_H_INCLUDED
#define PDSP_SCOREMESSAGE_H_INCLUDED

#include "../flags.h"
#include <vector>
#include <algorithm>
#include <assert.h>
#include "../DSP/pdspFunctions.h"

namespace pdsp{

//---------------------------------------------------------------------------
    /*!
    @brief Basic message block for scoring.
    
    This class rapresent a message contained inside the vector of a ScoreCell, having a time of execution, a value that will be interpreted by GateSequencer or ValueSequencer and the index of the output lane choosen between the one available of the ScoreSection that contains the ScoreCell.

    */ 
class ScoreMessage{
public:

    /*!
    @brief default values are time = 0.0, value = 0.0f, lane = 0
    */ 
    ScoreMessage();
    
    /*!
    @brief for this constructor lane = 0
    */ 
    ScoreMessage(double time, float value );
    
    ScoreMessage(double time, float value, int lane);
    ScoreMessage(const ScoreMessage &other);
    ScoreMessage& operator= (const ScoreMessage &other);
    ~ScoreMessage();
    /*!
    @brief time in bars of the execution, relative to the ScoreCell start
    */     
    double   time;    
    /*!
    @brief value to be output. GateSequencer and ValueSequencer interpret the received value in different manners.
    
    Value to be output. GateSequencer and ValueSequencer interpret the received value in different manners. GateSequencer emits a trigger on for values > 0.0f and a trigger off for value <= 0.0f. ValueSequencer constantly outputs a value, the sequenced value is changed at the right time, eventually the change is slewed if the slew is active.
    */     
    float    value;
    /*!
    @brief  the index of the output lane choosen between the one available of the ScoreSection that contains the ScoreCell.
    */     
    int      lane;
};


bool messageSort(const ScoreMessage &lhs, const ScoreMessage &rhs );



}//END NAMESPACE


#endif //PDSP_SCOREMESSAGE_H_INCLUDED
