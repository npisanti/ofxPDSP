
// BLEPFunc.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

// Based onWill Pirke's book "Designing Software Synthesizer Plugins in c++".


#ifndef PDSP_OSC_BLEPFUNC_H_INCLUDED
#define PDSP_OSC_BLEPFUNC_H_INCLUDED

#include "../../../pdspFunctions.h"
#include "../../../pdspConstants.h"


namespace pdsp{


    //--------------------BLEP CODE------------------------------------------------
 	inline float BLEPn(const float* BLEPTable, float tableFakeLen,
		float modulo, float inc, float height,
		bool risingEdge, float pointsPerSide, bool interpolate = false){

		float BLEP = 0.0; //return value
		float t = 0.0; //distance from discontinuity
        float tableCenter = tableFakeLen * 0.5 - 1.0;

		//LEFT side of the edge
		//-1 < t < 0
		for (int i = 1; i <= static_cast<int>(pointsPerSide); ++i){
			if (modulo > 1.0 - static_cast<float>(i)* inc){
				t = (modulo - 1.0) / (pointsPerSide * inc);		//calculate distance
				float index = (1.0 + t)*tableCenter; //residual index location
				if (interpolate){
					int index_int = static_cast<int> (index);
					float fract = index - index_int;
					BLEP = interpolate_linear(BLEPTable[index_int], BLEPTable[index_int + 1], fract);
				}
				else{
					BLEP = BLEPTable[static_cast<int>(index)]; //truncation
				}
				if (!risingEdge) BLEP *= -1.0;
				return BLEP*height;
			}
		}
		//RIGHT side of the edge
		//0 <= t < 1
		for (int i = 1; i <= static_cast<int>(pointsPerSide); ++i){
			if (modulo < static_cast<float>(i)* inc){
				t = modulo / (pointsPerSide * inc);		//calculate the distance
				float index = t*tableCenter + (tableCenter + 1.0); //residual index location
				if (interpolate){
					int index_int = static_cast<int>(index);
					float fract = index - index_int;
                    BLEP = interpolate_linear(BLEPTable[index_int], BLEPTable[index_int + 1], fract);
					//our table have a unit more with the same value as BLEPTable[0], so we're simulating circular buffer this way
				}
				else{
					BLEP = BLEPTable[static_cast<int>(index)];
				}
				if (!risingEdge) BLEP += -1.0;
				return BLEP * height;
			}
		}

		return 0.0; //no BLEP residual
	}   
    
	inline void BLEP4096WaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, float inc,
		const float* BLEPTable, bool risingEdge, float pointsPerSide, bool interpolate = false){		
		for (int n = 0; n < bufferSize; ++n){
			outputBuffer[n] += BLEPn(BLEPTable, 4096.0, phaseBuffer[n], inc, 1.0, risingEdge, pointsPerSide, interpolate);
		}
	}

	inline void BLEP4096FMWaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, const float* incBuffer,
		const float* BLEPTable, bool risingEdge, float pointsPerSide, bool interpolate = false){
		for (int n = 0; n < bufferSize; ++n){
			outputBuffer[n] += BLEPn(BLEPTable, 4096.0, phaseBuffer[n], fabs(incBuffer[n]), 1.0, risingEdge, pointsPerSide, interpolate);
		}
	}

    inline void BLEPWaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, float inc,
                                    const float* BLEPTable, bool risingEdge, float pointsPerSide, float tableLength, bool interpolate = false){
                                        
        float absinc = fabs(inc);
        for (int n = 0; n < bufferSize; ++n){
            outputBuffer[n] += BLEPn(BLEPTable, tableLength, phaseBuffer[n], absinc, 1.0, risingEdge, pointsPerSide, interpolate);
        }
    }
    
    inline void BLEPFMWaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, const float* incBuffer,
                                      const float* BLEPTable, bool risingEdge, float pointsPerSide, float tableLength, bool interpolate = false){
        for (int n = 0; n < bufferSize; ++n){
            outputBuffer[n] += BLEPn(BLEPTable, tableLength, phaseBuffer[n], fabs(incBuffer[n]), 1.0, risingEdge, pointsPerSide, interpolate);
        }
    }
	//--------------------------------  ----------------------------------------------------


}


#endif  // PDSP_OSC_BLEPFUNC_H_INCLUDED
