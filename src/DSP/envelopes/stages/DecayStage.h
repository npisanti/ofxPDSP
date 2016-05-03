
// DecayStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_DECAYSTAGE_H_INCLUDED
#define PDSP_ENVSTAGE_DECAYSTAGE_H_INCLUDED

#include "EnvelopeStage.h"

namespace pdsp{

class DecayStage : public virtual EnvelopeStage{
    
/*!
    @cond HIDDEN_SYMBOLS
*/
public:
        DecayStage(){
                decayTimeMs = 50.0;
                decayTCO = EnvelopeStage::analogDecayTCO;
                calculateDecayTime();
        };
/*!
    @endcond
*/

        /*!
        @brief sets a value that defines the decay curve, default is exp(-4.95) .
        @param[in] decayTCO decay TCO value
        */
        void setDecayTCO(float decayTCO){
                this->decayTCO = decayTCO;
                calculateDecayTime();
        }
/*!
    @cond HIDDEN_SYMBOLS
*/
protected:
        float decayTimeMs;
        float decayTCO;	//TCO set the curve
        float decayCoeff;
        float decayOffset;
        float decayNextStageLevel;

        virtual void setDecayTime(float decayTimeMs, float nextStageLevel){
                this->decayNextStageLevel = nextStageLevel;
                if(decayTimeMs <= 0.0f){ decayTimeMs = PDSP_MIN_ENVSTAGE_MS; } // to avoid divide by zero into coeff calculations
                this->decayTimeMs = decayTimeMs;
                calculateDecayTime();
        }
        
        void calculateDecayTime(){
                float samples = sampleRate * decayTimeMs*0.001f;
                decayCoeff = exp(-log((1.0f + decayTCO) / decayTCO) / samples);
                decayOffset = (decayNextStageLevel - decayTCO) * (1.0f - decayCoeff);
        }

        inline_f void Decay(int& stageSwitch, int nextStageId){
                envelopeOutput = decayOffset + envelopeOutput*decayCoeff;
                if (envelopeOutput <= decayNextStageLevel ){ // || decayTimeMs <= 0.0f removed
                        //envelopeOutput = decayNextStageLevel; //decativated for changint intensity
                        stageSwitch = nextStageId;
                }
        }

        

        virtual ~DecayStage(){};
};


/*!
    @endcond
*/

}//END NAMESPACE


#endif  // PDSP_ENVSTAGE_DECAYSUSTAIN_H_INCLUDED
