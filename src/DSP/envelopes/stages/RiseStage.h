
// RiseStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_RISESTAGE_H_INCLUDED
#define PDSP_ENVSTAGE_RISESTAGE_H_INCLUDED

#include "EnvelopeStage.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
class RiseStage : public virtual EnvelopeStage{
public:

        RiseStage(){
                riseTimeMs = 50.0;
                riseTCO = exp(-4.95); //digital 
                calculateRiseTime();
        };



        void setRiseTCO(float riseTCO){
                this->riseTCO = riseTCO;
                calculateRiseTime();
        }

protected:

        float riseTimeMs;
        float riseTCO;	//TCO set the curve
        float riseCoeff;
        float riseOffset;
        float riseNextStageLevel;

        virtual void setRiseTime(float riseTimeMs, float nextStageLevel){
                this->riseNextStageLevel = nextStageLevel;
                if(riseTimeMs <= 0.0f){ riseTimeMs = PDSP_MIN_ENVSTAGE_MS; } // to avoid divide by zero into coeff calculations
                this->riseTimeMs = riseTimeMs;
                calculateRiseTime();
        }
        
        void calculateRiseTime(){
                float samples = sampleRate * riseTimeMs*0.001f;
                riseCoeff = exp(-log((1.0f + riseTCO) / riseTCO) / samples);
                riseOffset = (riseNextStageLevel + riseTCO) * (1.0f - riseCoeff);
        }

        inline_f void Rise(int& stageSwitch, int nextStageId){
                envelopeOutput = riseOffset + envelopeOutput*riseCoeff;
                if (envelopeOutput >= riseNextStageLevel ){ // || riseTimeMs <= 0.0f removed
                        //envelopeOutput = riseNextStageLevel; //decativated for changint intensity
                        stageSwitch = nextStageId;
                }
        }

        

        virtual ~RiseStage(){};
};

/*!
    @endcond
*/

}//END NAMESPACE



#endif  // PDSP_ENVSTAGE_RISESTAGE_H_INCLUDED
