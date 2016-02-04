
// ReleaseStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_RELEASE_H_INCLUDED
#define PDSP_ENVSTAGE_RELEASE_H_INCLUDED

#include "EnvelopeStage.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
	class ReleaseStage : public virtual EnvelopeStage{
	public:

		ReleaseStage(){
			releaseTimeMs = 50.0;
			releaseTCO = exp(-4.95); //digital 
			calculateReleaseTime();
		};

	

		void setReleaseTCO(float releaseTCO){
			this->releaseTCO = releaseTCO;
			calculateReleaseTime();
		}


	protected:

		float releaseTimeMs;
		float releaseTCO;	//TCO set the curve
		float releaseCoeff;
		float releaseOffset;


		virtual void setReleaseTime(float releaseTimeMs){
			this->releaseTimeMs = releaseTimeMs;
			calculateReleaseTime();
		}

		void calculateReleaseTime(){
			float samples = sampleRate * releaseTimeMs*0.001f;
			releaseCoeff = exp(-log((1.0f + releaseTCO) / releaseTCO) / samples);
			releaseOffset = (- releaseTCO) * (1.0f - releaseCoeff);
		}

		inline_f void Release(int& stageSwitch, int offStageId){

			envelopeOutput = releaseOffset + envelopeOutput*releaseCoeff;
			if (envelopeOutput <= 0.0f || releaseTimeMs <= 0.0f){
				envelopeOutput = 0.0f;
				stageSwitch = offStageId;
			}

		}

		virtual ~ReleaseStage(){};
	};

/*!
    @endcond
*/

}


#endif  // PDSP_ENVSTAGE_RELEASE_H_INCLUDED
