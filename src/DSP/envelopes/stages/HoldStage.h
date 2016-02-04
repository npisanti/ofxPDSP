
// HoldStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_HOLD_H_INCLUDED
#define PDSP_ENVSTAGE_HOLD_H_INCLUDED


#include "EnvelopeStage.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
class HoldStage : public virtual EnvelopeStage{

public:
        HoldStage(){
                holdTimeMs = 40.0f;
                counter = 0;
                calculateHoldTimeAndReset();
        };


protected:
        float holdTimeMs;
        float holdLevel;
        int counter;
        int holdSamples;

        virtual void setHoldTime(float holdTimeMs, float holdLevel){
                this->holdLevel = holdLevel;
                this->holdTimeMs = holdTimeMs;
                calculateHoldTimeAndReset();
        }
        
        void calculateHoldTimeAndReset(){
                holdSamples = sampleRate * holdTimeMs*0.001f;
                counter = 0;
        }

        inline_f void Hold(int& stageSwitch, int nextStageId){
                envelopeOutput = holdLevel;
                if (counter++ <= holdSamples || holdTimeMs <= 0.0f){
                        stageSwitch = nextStageId;
                }
        }

        

        virtual ~HoldStage(){};
};

/*!
    @endcond
*/

}//END NAMESPACE








#endif  // PDSP_ENVSTAGE_HOLD_H_INCLUDED
