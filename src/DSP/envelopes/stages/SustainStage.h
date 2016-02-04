
// SustainStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_SUSTAINSTAGE_H_INCLUDED
#define PDSP_ENVSTAGE_SUSTAINSTAGE_H_INCLUDED

#include "EnvelopeStage.h"


namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
	class SustainStage : public virtual EnvelopeStage{
	public:
		SustainStage(){
			sustainLevel = 0.0f;
			output_bias.set(0.0f);
		}


		ValueNode output_bias;

	protected:
		float sustainLevel;


		void setSustainLevel(float sustainLevel){
			this->sustainLevel = sustainLevel * intensity;
			output_bias.set(-sustainLevel);
		}


		inline_f void Sustain(){
			envelopeOutput = sustainLevel;
		}

		virtual ~SustainStage(){};
	};
/*!
    @endcond
*/
}




#endif  // PDSP_ENVSTAGE_SUSTAINSTAGE_H_INCLUDED
