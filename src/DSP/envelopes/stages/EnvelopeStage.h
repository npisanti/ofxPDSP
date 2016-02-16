
// EnvelopeStage.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_ENVSTAGE_ENVELOPESTAGE_H_INCLUDED
#define PDSP_ENVSTAGE_ENVELOPESTAGE_H_INCLUDED

#include "../../../flags.h"

namespace pdsp {

/*!
    @cond HIDDEN_SYMBOLS
*/
    class EnvelopeStage{
	public:

		EnvelopeStage(){
			envelopeOutput = 0.0f;
			sampleRate = 44100.0;
            intensity = 1.0f;
		};
	
	protected:

        float intensity;
		float sampleRate;
		float envelopeOutput;

		void setEnvelopeSampleRate(double sampleRate){
			this->sampleRate = sampleRate;
		}

		virtual ~EnvelopeStage(){};

	};
/*!
    @endcond
*/

}



#endif  // PDSP_ENVSTAGE_ENVELOPESTAGE_H_INCLUDED
