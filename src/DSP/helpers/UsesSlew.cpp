
#include "UsesSlew.h"

pdsp::UsesSlew::UsesSlew(){
    slewInitValue = 0.0f;
    slewLastValue = 0.0f;
    slewTimeMod = 0.0f;
    
    slewMode = Time;
    setSlewRateModeReference(12.0f);
    
    setSlewTime(50.0f);
    slewAccumulator = 0.0f;
    slewValueInterval = 0.0f;
    slewTCO  = exp(-4.95);
    slewRun = false;
    
}


void pdsp::UsesSlew::setSlewTime(float slewTimeMs, SlewMode_t mode){
    this->slewTimeMs = slewTimeMs;
    this->slewMode = mode;
    slewTimeMod = 1.0f;
}

void pdsp::UsesSlew::deactivateSlew(){
    slewTimeMod = 0.0f;
}


void pdsp::UsesSlew::prepareSlew(float sampleRate, float initValue){
    this->sampleRate = sampleRate;
    this->slewInitValue = initValue;
    this->slewLastValue = initValue;
}

void pdsp::UsesSlew::setInitValue(float value){
    slewInitValue = value;
    slewLastValue = value;
}

void pdsp::UsesSlew::setSlewRateModeReference(float valueReference){
    slewTimeRangeMultiplier = 1.0f / valueReference;
}


void pdsp::UsesSlew::valueChange( const float &slewValueDest ) noexcept{
    
    if(slewTimeMod <=0.0f){
        slewLastValue = slewValueDest;
        slewRun = false;
    }else{
        
        slewValueStart = slewLastValue;
        this->slewValueDest = slewValueDest;
        slewAccumulator = 0.0f;
        slewValueInterval = slewValueDest - slewValueStart;
        
        float timeModFinal;
        if(slewMode == Rate){
            timeModFinal = slewTimeMod  * std::abs(slewValueInterval) * slewTimeRangeMultiplier;
        }else{
            timeModFinal = slewTimeMod;
        }
        
        float samples = sampleRate * slewTimeMs * 0.001f * timeModFinal;
        slewCoeff =  exp(-log((1.0f + slewTCO) / slewTCO) / samples);
        slewOffset = (1.0f + slewTCO) * (1.0f - slewCoeff);
        
        slewRun = true;
    }
   
}


void pdsp::UsesSlew::runSlewBlock(float* &buffer, const int &start, const int &stopExclusive) noexcept{
    
	int n = start;
	
	for( ; n<stopExclusive; ++n){
		buffer[n] = slewValueStart + slewAccumulator * slewValueInterval;
		slewAccumulator = slewOffset + (slewAccumulator * slewCoeff);
		if(slewAccumulator>=1.0f){
			slewAccumulator = 1.0f;
			slewRun = false;
			break;
		}
	}
	        
	if( !slewRun && n<stopExclusive){
        n++; //as the break stops the increments
		ofx_Aeq_S_range(buffer, slewValueDest, n, stopExclusive);
        slewLastValue = slewValueDest;
                
	}else if(start!=stopExclusive) {
        slewLastValue = buffer[stopExclusive-1];
        //std::cout<<"last value set to  "<<slewLastValue<<"\n";
    }

        
}
