

#include "SRDelay.h"




pdsp::SRDelay::SRDelay() : SRDelay(1000.0f){}

                
pdsp::SRDelay::SRDelay( float timeMs){

        addInput("signal", input);
        addInput("time", in_time_ms);
        addOutput("signal", output);
        updateOutputNodes();
    
        sampleRate = 44100.0;
        msToSamplesMultiplier = sampleRate * 0.001;
        //index = 0;
        maxDelayTimeMs = timeMs;
        delayBuffer = nullptr;
        writeIndex = 0;
        in_time_ms.setDefaultValue ( timeMs * 0.5f);
        boundaries = true;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}
float pdsp::SRDelay::meter_time() const{
    return timeMeter.load();
}

pdsp::Patchable& pdsp::SRDelay::set( float timeMs){
    in_time_ms.setDefaultValue(timeMs);
    return *this;
}

pdsp::Patchable& pdsp::SRDelay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SRDelay::in_time(){
    return in("time");
}
        
pdsp::Patchable& pdsp::SRDelay::out_signal(){
    return out("signal");
}

void pdsp::SRDelay::updateBoundaries(){
        
        if(boundaries){
                float low = (1200.0f * globalBufferSize ) / this->sampleRate; //1.2 times the expected buffer size
                float high = maxDelayTimeMs - (1000.0f/this->sampleRate);
                in_time_ms.enableBoundaries( low, high);	
        }else{
                in_time_ms.disableBoundaries();
        }

}


void pdsp::SRDelay::setMaxTime(float timeMs){
        maxDelayTimeMs = timeMs;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }	
}

void pdsp::SRDelay::prepareUnit( int expectedBufferSize, double sampleRate){
        
        this->sampleRate = sampleRate;
        //index = 0.0;
        msToSamplesMultiplier = this->sampleRate * 0.001;
        
        maxDelayTimeSamples = static_cast<int> (maxDelayTimeMs * msToSamplesMultiplier) ; 
        maxDelayTimeSamples_f = static_cast<float> ( maxDelayTimeSamples );

        initDelayBuffer();
        updateBoundaries();

}

void pdsp::SRDelay::releaseResources(){
        if (delayBuffer != nullptr){
                delete[] delayBuffer;
                delayBuffer = nullptr;
        }
}

void pdsp::SRDelay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples +1 );//1 is the guard point
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}


void pdsp::SRDelay::process(int bufferSize) noexcept{
        
        
        float* outputBuffer = getOutputBufferToFill(output);
        int timeBufferState;
        const float* timeBuffer = processInput(in_time_ms, timeBufferState);

        if(timeBufferState==Changed){
                process_once(timeBuffer);
                
        }

        switch(timeBufferState){
        case AudioRate:
                read_audio<true> (outputBuffer, timeBuffer, bufferSize);
                break;
        default:
                read_audio<false> (outputBuffer, timeBuffer, bufferSize);
                break;
        }

        write_audio(outputBuffer, bufferSize);

        timeMeter.store( timeBuffer[0] );
}



void pdsp::SRDelay::process_once(const float* timeBuffer){
        readIndex = static_cast<float>(writeIndex) - timeBuffer[0]*msToSamplesMultiplier;
        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
}


template<bool timeAR>
void pdsp::SRDelay::read_audio(float* &outputBuffer, const float* &timeBuffer, const int &bufferSize){
        
        for(int n=0; n<bufferSize; ++n){
                
                if(timeAR){
                        readIndex = static_cast<float>(writeIndex+n) - timeBuffer[n]*msToSamplesMultiplier;
                        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
                        else if(readIndex>=maxDelayTimeSamples){ readIndex = readIndex - maxDelayTimeSamples; }
                }
                
                int index_int = static_cast<int> (readIndex);
                float mu = readIndex - index_int;
                float x1 = delayBuffer[index_int];
                float x2 = delayBuffer[index_int+1];

                outputBuffer[n] = interpolate_linear( x1, x2, mu );
                
                if(!timeAR){
                        //readIndex_i++;
                        //if (readIndex_i == maxDelayTimeSamples){ readIndex_i = 0; };
                        
                        readIndex += 1.0f;
                        if(readIndex >= maxDelayTimeSamples_f){
                                readIndex -= maxDelayTimeSamples_f;
                        }
                }
        }
}


void pdsp::SRDelay::write_audio(float* &outputBuffer, const int& bufferSize){
        
        int inputBufferState;
        const float* inputBuffer = processInput(input, inputBufferState);
        
        for(int n=0; n<bufferSize; ++n){
                
                delayBuffer[writeIndex] = inputBuffer[n];
                
                if (++writeIndex > maxDelayTimeSamples){
                        writeIndex = 1;
                        delayBuffer[0] = delayBuffer[maxDelayTimeSamples]; //so we don't need another branch for linear interpolation
                }
        }
        
}


