
// SequencerMessage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef PDSP_SCOREMESSAGE_H_INCLUDED
#define PDSP_SCOREMESSAGE_H_INCLUDED

#include "../flags.h"
#include <vector>
#include <algorithm>
#include <assert.h>
#include "../DSP/pdspFunctions.h"

/*!
    @cond HIDDEN_SYMBOLS
*/

namespace pdsp{

    /*!
    @brief Basic message block for scoring.
    
    This class rapresent a message contained inside the vector of a ScoreCell, having a time of execution, a value that will be interpreted by GateSequencer or ValueSequencer and the index of the output lane choosen between the one available of the ScoreSection that contains the ScoreCell.

    */ 

    enum MessageType{ MValue, MSlew, MLegato };

class SequencerMessage{
public:

    /*!
    @brief default values are time = 0.0, value = 0.0f, lane = 0
    */ 
    SequencerMessage();
    
    /*!
    @brief for this constructor lane = 0
    */ 
    SequencerMessage(double time, float value );
    
    SequencerMessage(double time, float value, int lane);
    SequencerMessage(double time, float value, int lane, MessageType mtype );
    SequencerMessage(const SequencerMessage &other);
    SequencerMessage& operator= (const SequencerMessage &other);
    ~SequencerMessage();
    /*!
    @brief time in bars of the execution, relative to the ScoreCell start
    */     
    double time;  
      
    /*!
    @brief value to be output. GateSequencer and ValueSequencer interpret the received value in different manners.
    
    Value to be output. GateSequencer and ValueSequencer interpret the received value in different manners. GateSequencer emits a trigger on for values > 0.0f and a trigger off for value <= 0.0f. ValueSequencer constantly outputs a value, the sequenced value is changed at the right time, eventually the change is slewed if the slew is active.
    */     
    float value;
    
    /*!
    @brief  the index of the output lane choosen between the one available of the ScoreSection that contains the ScoreCell.
    */     
    int lane;

    /*!
    @brief type of message, for implementing some GateSequencer and ValueSequencer behaviors
    */         
    MessageType mtype;
};


bool messageSort(const SequencerMessage &lhs, const SequencerMessage &rhs );

}//END NAMESPACE

/*!
    @endcond
*/

#endif //PDSP_SCOREMESSAGE_H_INCLUDED
