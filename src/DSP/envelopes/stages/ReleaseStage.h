
// ReleaseStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_RELEASE_H_INCLUDED
#define PDSP_ENVSTAGE_RELEASE_H_INCLUDED

#include "EnvelopeStage.h"

namespace pdsp{

	class ReleaseStage : public virtual EnvelopeStage{
	public:
/*!
    @cond HIDDEN_SYMBOLS
*/
		ReleaseStage(){
			releaseTimeMs = 50.0;
			releaseTCO = EnvelopeStage::analogDecayTCO;
			calculateReleaseTime();
		};

	
/*!
    @endcond
*/

        /*!
        @brief sets a value that defines the release curve, default is exp(-4.95) .
        @param[in] releaseTCO release TCO value
        */
		void setReleaseTCO(float releaseTCO){
			this->releaseTCO = releaseTCO;
			calculateReleaseTime();
		}

/*!
    @cond HIDDEN_SYMBOLS
*/
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
