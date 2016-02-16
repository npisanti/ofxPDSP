
// AttackStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_ATTACK_H_INCLUDED
#define PDSP_ENVSTAGE_ATTACK_H_INCLUDED

#include "EnvelopeStage.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
	class AttackStage : public virtual EnvelopeStage{
	
	public:

		AttackStage(){
			attackTimeMs = 50.0f;
			attackTCO = 0.99999f; //digital 
			calculateAttackTime();
		};

		void setAttackTCO(float attackTCO){
			this->attackTCO = attackTCO;
			calculateAttackTime();
		};
	

	protected:
		float attackTimeMs;
		float attackTCO;	//TCO set the curve
		float attackCoeff;
		float attackOffset;
	
		void setAttackTime(float attackTimeMs){
            if(attackTimeMs <= 0.0f){ attackTimeMs = PDSP_MIN_ENVSTAGE_MS; } // to avoid divide by zero into coeff calculations
			this->attackTimeMs = attackTimeMs;
			calculateAttackTime();
		};

		void calculateAttackTime(){
			float samples = sampleRate * attackTimeMs*0.001f;
			attackCoeff = exp(-log((1.0f + attackTCO) / attackTCO) / samples);
			attackOffset = (1.0f + attackTCO) * (1.0f - attackCoeff);
		}

		inline_f void Attack(int& stageSwitch, int nextStageId){
			if (envelopeOutput >= intensity ){
				stageSwitch = nextStageId;
			}else{
                envelopeOutput = attackOffset + envelopeOutput*attackCoeff;   
                envelopeOutput = (envelopeOutput > intensity) ? intensity : envelopeOutput; // this should be compiled into branchless code
            }
		}

		virtual ~AttackStage(){};
	};


/*!
    @endcond
*/

} // pdsp namespace end

#endif  // PDSP_ENVSTAGE_ATTACK_H_INCLUDED
