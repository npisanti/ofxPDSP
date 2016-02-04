

#include "DampedDelay.h"


pdsp::DampedDelay::DampedDelay() : DampedDelay(1000.0f){};

                
pdsp::DampedDelay::DampedDelay( float timeMs){
        addInput("signal", input);
        addInput("time", in_time_ms);
        addInput("feedback", input_feedback);
        addInput("damping", input_damping);
        addOutput("signal", output);
        updateOutputNodes();
        
        sampleRate = 44100.0;
        msToSamplesMultiplier = sampleRate * 0.001;
        z1 = 0.0f;
        feedback = 0.0;
        g = 0.33f;
        setDamping(g);

        maxDelayTimeMs = timeMs;
        delayBuffer = nullptr;
        writeIndex = 0;
        in_time_ms.setDefaultValue ( timeMs * 0.5f);
        input_feedback.setDefaultValue(0.0f);
        input_damping.setDefaultValue(0.0f);
        boundaries = true;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
        
};

pdsp::Patchable& pdsp::DampedDelay::set( float timeMs, float feedback, float damping){
    in_time_ms.setDefaultValue(timeMs);
    input_feedback.setDefaultValue(feedback);
    input_damping.setDefaultValue(damping);
    return *this;
}

float pdsp::DampedDelay::meter_time() const{
    return timeMeter.load();
}

pdsp::Patchable& pdsp::DampedDelay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::DampedDelay::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::DampedDelay::in_time(){
    return in("time");
}

pdsp::Patchable& pdsp::DampedDelay::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::DampedDelay::in_damping(){
    return in("damping");
}

void pdsp::DampedDelay::changeInterpolator(Interpolator_t interpolatorMode){
        interShell.changeInterpolator(interpolatorMode);
}
/*
void pdsp::DampedDelay::timeBoundaryEnabled(bool enable){
        this->boundaries = enable;
                
        updateBoundaries();
}
*/

void pdsp::DampedDelay::updateBoundaries(){
        
        if(boundaries){
                float low = 3000.0f / this->sampleRate;
                float high = maxDelayTimeMs - (1000.0f/this->sampleRate);
                in_time_ms.enableBoundaries( low, high);
        }else{
                in_time_ms.disableBoundaries();
        }

}

void pdsp::DampedDelay::setDamping(float hiDamp){        
        g = hiDamp;
        if (g >= 1.0){
                g = 0.99;
        }
        gLPF = g * (1.0f - feedback);

}

void pdsp::DampedDelay::setMaxTime(float timeMs){
        maxDelayTimeMs = timeMs;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }	
}


void pdsp::DampedDelay::setFeedback(float feedback){ 
        this->feedback = feedback; 
        setDamping(g);
};


void pdsp::DampedDelay::prepareUnit ( int expectedBufferSize, double sampleRate ){
        z1 = 0.0f;
        this->sampleRate = sampleRate;
        
        msToSamplesMultiplier = this->sampleRate * 0.001;
        
        maxDelayTimeSamples = static_cast<int> (maxDelayTimeMs * msToSamplesMultiplier) +1; //1 is the guard point
        maxDelayTimeSamples_f = static_cast<float>(maxDelayTimeSamples);

        initDelayBuffer();
        updateBoundaries();

}

void pdsp::DampedDelay::releaseResources(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
}

void pdsp::DampedDelay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples);
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}


void pdsp::DampedDelay::process(int bufferSize) noexcept{
        
        int changed;
        const float* feedbackBuffer = processInput(input_feedback, changed);
        if(changed){
                setFeedback(feedbackBuffer[0]);
        }
        
        
        const float* dampingBuffer = processInput(input_damping, changed);
        if(changed){
                setDamping(dampingBuffer[0]);
        }

        int inputBufferState;
        const float* inputBuffer = processInput( input, inputBufferState );
        int timeBufferState;
        const float* timeBuffer = processInput( in_time_ms, timeBufferState );

        if(timeBufferState==Changed){
                process_once(timeBuffer);
        }

        int switcher = inputBufferState + timeBufferState*4;

        switch ( switcher & processAudioBitMask ) {
        case audioFFFF:
                process_audio<false, false> (inputBuffer, timeBuffer, bufferSize);
                break;
        case audioTFFF:
                process_audio<true, false> (inputBuffer, timeBuffer, bufferSize);
                break;
        case audioFTFF:
                process_audio<false, true> (inputBuffer, timeBuffer, bufferSize);
                break;
        case audioTTFF:
                process_audio<true, true> (inputBuffer, timeBuffer, bufferSize);
                break;
        }
        
        timeMeter.store( timeBuffer[0] );
}



void pdsp::DampedDelay::process_once(const float* timeBuffer)noexcept{
        readIndex = static_cast<float>(writeIndex) - timeBuffer[0]*msToSamplesMultiplier;
        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
}


template<bool inputAR, bool timeAR>
void pdsp::DampedDelay::process_audio(const float* inputBuffer, const float* timeBuffer, int bufferSize)noexcept{
     

        float* outputBuffer = getOutputBufferToFill( output );

        for(int n=0; n<bufferSize; ++n){
                if(timeAR){
                        readIndex = static_cast<float>(writeIndex) - timeBuffer[n]*msToSamplesMultiplier;
                        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
                }
                
                outputBuffer[n] = interShell.interpolate(delayBuffer, readIndex, maxDelayTimeSamples);
                
                //filter code
                float readValue = (outputBuffer[n] + gLPF * z1);
                z1 = readValue;
                //end filter code
                
                if(inputAR){
                        delayBuffer[writeIndex] = inputBuffer[n] + readValue * feedback;
                }else{
                        delayBuffer[writeIndex] =  readValue * feedback;
                }
                
                
                if (++writeIndex > maxDelayTimeSamples){
                        writeIndex = 1;
                        delayBuffer[0] = delayBuffer[maxDelayTimeSamples]; //so we don't need another branch for linear interpolation
                }
        
                if(!timeAR){
                        readIndex += 1.0f;
                        if(readIndex >= maxDelayTimeSamples_f){
                                readIndex -= maxDelayTimeSamples_f;
                        }
                }
        }
}
