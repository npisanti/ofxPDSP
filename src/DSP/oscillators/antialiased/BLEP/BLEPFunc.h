
// BLEPFunc.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

// Based on Will Pirke's book "Designing Software Synthesizer Plugins in c++", but working only on falling edges.


#ifndef PDSP_OSC_BLEPFUNC_H_INCLUDED
#define PDSP_OSC_BLEPFUNC_H_INCLUDED

#include "../../../pdspFunctions.h"
#include "../../../pdspConstants.h"


namespace pdsp{


    //--------------------BLEP CODE------------------------------------------------
 	inline_f float BLEPn(const float* BLEPTable, const float &tableFakeLen,
		const float &modulo, const float &inc, const float &height,
		const float &pointsPerSide, const int &pointsPerSide_int, bool interpolate, const float &tableCenter, const float & oneSlashPointPerSide){

		float BLEP = 0.0f; //return value
		float t = 0.0f; //distance from discontinuity


		//LEFT side of the edge
		//-1 < t < 0
		for (int i = 1; i <= pointsPerSide_int; ++i){
			if (modulo > 1.0f - static_cast<float>(i) * inc){
				t = (modulo - 1.0f) * (oneSlashPointPerSide * inc);		//calculate distance
				float index = (1.0f + t)*tableCenter; //residual index location
				if (interpolate){
					int index_int = static_cast<int> (index);
					float fract = index - index_int;
					BLEP = interpolate_linear(BLEPTable[index_int], BLEPTable[index_int + 1], fract);
				}
				else{
					BLEP = BLEPTable[static_cast<int>(index)]; //truncation
				}

                BLEP *= -1.0f; // always falling edge
                BLEP *= height;
				return BLEP;
			}
		}
		//RIGHT side of the edge
		//0 <= t < 1
		for (int i = 1; i <= pointsPerSide_int; ++i){
			if (modulo < static_cast<float>(i) * inc){
				t = modulo * (oneSlashPointPerSide * inc);		//calculate the distance
				float index = t*tableCenter + (tableCenter + 1.0f); //residual index location
				if (interpolate){
					int index_int = static_cast<int>(index);
					float fract = index - index_int;
                    BLEP = interpolate_linear(BLEPTable[index_int], BLEPTable[index_int + 1], fract);
					//our table have a unit more with the same value as BLEPTable[0], so we're simulating circular buffer this way
				}
				else{
					BLEP = BLEPTable[static_cast<int>(index)];
				}

                BLEP *= -1.0f; // always falling edge
                BLEP *= height;
				return BLEP;
			}
		}

		return 0.0; //no BLEP residual
	}   

    inline void BLEPWaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, float inc,
                                    const float* BLEPTable, float pointsPerSide, float tableFakeLen, bool interpolate = false){
        float tableCenter = tableFakeLen * 0.5f - 1.0f;
        int pointsPerSide_i = static_cast<int>(pointsPerSide);
        float absinc = fabs(inc);
        float oneSlashPointPerSide = 1.0f / pointsPerSide;
        
        for (int n = 0; n < bufferSize; ++n){
            outputBuffer[n] += BLEPn(BLEPTable, tableFakeLen, phaseBuffer[n], absinc, 1.0f, pointsPerSide, pointsPerSide_i, interpolate, tableCenter, oneSlashPointPerSide);
        }
    }
    
    inline void BLEPFMWaveformDSP(int bufferSize, float* outputBuffer, const float* phaseBuffer, const float* incBuffer,
                                      const float* BLEPTable, float pointsPerSide, float tableFakeLen, bool interpolate = false){
        
        float tableCenter = tableFakeLen * 0.5f - 1.0f;
        int pointsPerSide_i = static_cast<int>(pointsPerSide);
        float oneSlashPointPerSide = 1.0f / pointsPerSide;     

        for (int n = 0; n < bufferSize; ++n){

            outputBuffer[n] += BLEPn(BLEPTable, tableFakeLen, phaseBuffer[n], fabs(incBuffer[n]), 1.0f, pointsPerSide, pointsPerSide_i, interpolate, tableCenter, oneSlashPointPerSide);
        }
    }
	//--------------------------------  ----------------------------------------------------


}


#endif  // PDSP_OSC_BLEPFUNC_H_INCLUDED
