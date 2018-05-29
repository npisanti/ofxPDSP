

#include "LHDelay.h"


pdsp::LHDelay::LHDelay() : LHDelay( 300.0f ){};

                
pdsp::LHDelay::LHDelay( float timeMs ){
        addInput("signal", input);
        addInput("time", in_time_ms);
        addOutput("signal", output);
        updateOutputNodes();

        sampleRate = 44100.0;
        msToSamplesMultiplier = sampleRate * 0.001;

        maxDelayTimeMs = timeMs;
        delayBuffer = nullptr;

        in_time_ms.setDefaultValue ( timeMs * 0.5f);

        offset = 0;
        writeIndex = 0;
        readIndex = 0;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
};

void pdsp::LHDelay::prepareUnit ( int expectedBufferSize, double sampleRate ){
        this->sampleRate = sampleRate;
        
        msToSamplesMultiplier = this->sampleRate * 0.001;
        
        maxDelayTimeSamples = static_cast<int> (maxDelayTimeMs * msToSamplesMultiplier) +1; //1 is the guard point

        initDelayBuffer();
        updateBoundaries();
        
        readIndex = 0;
        silenceSamples = 0;
}

void pdsp::LHDelay::updateBoundaries(){
        float low = 0.0f;
        float high = maxDelayTimeMs - (1000.0f/this->sampleRate);
        in_time_ms.enableBoundaries( low, high);
}

pdsp::Patchable& pdsp::LHDelay::set( float timeMs ){
        in_time_ms.setDefaultValue( timeMs );
        return *this;
}

pdsp::Patchable& pdsp::LHDelay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::LHDelay::in_time(){
    return in("time");
}

pdsp::Patchable& pdsp::LHDelay::out_signal(){
    return out("signal");
}

void pdsp::LHDelay::releaseResources(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
}

void pdsp::LHDelay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples);
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}

void pdsp::LHDelay::setMaxTime(float timeMs){
        maxDelayTimeMs = timeMs;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }	
}

void pdsp::LHDelay::process(int bufferSize) noexcept{
        
        int timeChanged;
        const float* timeBuffer = processInput( in_time_ms, timeChanged );
        
        if(timeChanged){
            offset = std::ceil( timeBuffer[0] * msToSamplesMultiplier );
            readIndex = writeIndex - offset;
            while( readIndex < 0 ){ readIndex += maxDelayTimeSamples; } 
        }

        int inputBufferState;
        const float* inputBuffer = processInput( input, inputBufferState );
      
        switch (inputBufferState){
            
            case Unchanged: case Changed:
                
                if(silenceSamples > maxDelayTimeSamples){
                    setOutputToZero(output);
                }else{
                    float* outputBuffer = getOutputBufferToFill( output );
                    for(int n=0; n<bufferSize; ++n){
                        outputBuffer[n] = 0.0f;
                        delayBuffer[writeIndex] = 0.0f;
                        readIndex++;
                        readIndex = (readIndex==maxDelayTimeSamples) ? 0 : readIndex;
                        writeIndex++;
                        writeIndex = (writeIndex==maxDelayTimeSamples) ? 0 : writeIndex;
                    }                    
                    silenceSamples += bufferSize;
                }

            break;
            
            case AudioRate:
                silenceSamples = 0;
                float* outputBuffer = getOutputBufferToFill( output );
            
                for(int n=0; n<bufferSize; ++n){
                    outputBuffer[n] = delayBuffer[readIndex];
                    readIndex++;
                    readIndex = (readIndex==maxDelayTimeSamples) ? 0 : readIndex;

                    delayBuffer[writeIndex] = inputBuffer[n];
                    writeIndex++;
                    writeIndex = (writeIndex==maxDelayTimeSamples) ? 0 : writeIndex;
                }
            break;
                        
        }
}

