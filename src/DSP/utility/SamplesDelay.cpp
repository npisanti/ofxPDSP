

#include "SamplesDelay.h"


pdsp::SamplesDelay::SamplesDelay() : SamplesDelay( 128 ){};

                
pdsp::SamplesDelay::SamplesDelay( int samples ){
        addInput("signal", input);
        addOutput("signal", output);
        updateOutputNodes();
        
        maxDelayTimeSamples = samples;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
};

pdsp::Patchable& pdsp::SamplesDelay::set( int samples){
    setSamples(samples);
    return *this;
}

void pdsp::SamplesDelay::setSamples( int samples){
    maxDelayTimeSamples = samples;
    
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::SamplesDelay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SamplesDelay::out_signal(){
    return out("signal");
}


void pdsp::SamplesDelay::prepareUnit ( int expectedBufferSize, double sampleRate ){
        initDelayBuffer();
        index = 0;
        silenceSamples = 0;
}

void pdsp::SamplesDelay::releaseResources(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
}

void pdsp::SamplesDelay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples);
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}


void pdsp::SamplesDelay::process(int bufferSize) noexcept{
        
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
                        delayBuffer[index] = 0.0f;
                        index++;
                        if(index == maxDelayTimeSamples){
                            index = 0;
                        }
                    }                    
                    silenceSamples += bufferSize;
                }

            break;
            
            case AudioRate:
                silenceSamples = 0;
                float* outputBuffer = getOutputBufferToFill( output );
            
                for(int n=0; n<bufferSize; ++n){
                    outputBuffer[n] = delayBuffer[index];
                    delayBuffer[index] = inputBuffer[n];
                    index++;
                    if(index == maxDelayTimeSamples){
                        index = 0;
                    }
                }
            break;
                        
        }
      
}

