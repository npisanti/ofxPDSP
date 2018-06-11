

#include "AllPassDelay.h"


pdsp::AllPassDelay::AllPassDelay() : AllPassDelay(1000.0f){};

                
pdsp::AllPassDelay::AllPassDelay( float timeMs){
        addInput("signal", input);
        addInput("time", in_time_ms);
        addInput("feedback", input_feedback);
        addOutput("signal", output);
        updateOutputNodes();
        
        sampleRate = 44100.0;
        msToSamplesMultiplier = sampleRate * 0.001;

        g = 0.0f;
        outGcoeff = 1.0f;
        
        maxDelayTimeMs = timeMs;
        delayBuffer = nullptr;
        writeIndex = 0;
        in_time_ms.setDefaultValue ( timeMs * 0.5f);
        input_feedback.setDefaultValue(0.0f);

        boundaries = true;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
        
};

pdsp::Patchable& pdsp::AllPassDelay::set( float timeMs, float feedback){
    in_time_ms.setDefaultValue(timeMs);
    input_feedback.setDefaultValue(feedback);
    return *this;
}

float pdsp::AllPassDelay::meter_time() const{
    return timeMeter.load();
}

pdsp::Patchable& pdsp::AllPassDelay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::AllPassDelay::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::AllPassDelay::in_time(){
    return in("time");
}

pdsp::Patchable& pdsp::AllPassDelay::in_feedback(){
    return in("feedback");
}

void pdsp::AllPassDelay::changeInterpolator(Interpolator_t interpolatorMode){
        interShell.changeInterpolator(interpolatorMode);
}

void pdsp::AllPassDelay::updateBoundaries(){
        
        if(boundaries){
                float low = 3000.0f / this->sampleRate;
                float high = maxDelayTimeMs - (1000.0f/this->sampleRate);
                in_time_ms.enableBoundaries( low, high);
        }else{
                in_time_ms.disableBoundaries();
        }

}


void pdsp::AllPassDelay::setMaxTime(float timeMs){
        maxDelayTimeMs = timeMs;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }	
}


void pdsp::AllPassDelay::prepareUnit ( int expectedBufferSize, double sampleRate ){
        this->sampleRate = sampleRate;
        
        msToSamplesMultiplier = this->sampleRate * 0.001;
        
        maxDelayTimeSamples = static_cast<int> (maxDelayTimeMs * msToSamplesMultiplier) +1; //1 is the guard point
        maxDelayTimeSamples_f = static_cast<float>(maxDelayTimeSamples);

        initDelayBuffer();
        updateBoundaries();

}

void pdsp::AllPassDelay::releaseResources(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
}

void pdsp::AllPassDelay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples);
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}


void pdsp::AllPassDelay::process(int bufferSize) noexcept{
        
        int changed;
        const float* feedbackBuffer = processInput(input_feedback, changed);
        if(changed){
                g = feedbackBuffer[0];
                outGcoeff = 1.0f - g*g;
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



void pdsp::AllPassDelay::process_once(const float* timeBuffer)noexcept{
        readIndex = static_cast<float>(writeIndex) - timeBuffer[0]*msToSamplesMultiplier;
        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
}


template<bool inputAR, bool timeAR>
void pdsp::AllPassDelay::process_audio(const float* inputBuffer, const float* timeBuffer, int bufferSize)noexcept{
     

        float* outputBuffer = getOutputBufferToFill( output );

        for(int n=0; n<bufferSize; ++n){
                if(timeAR){
                        readIndex = static_cast<float>(writeIndex) - timeBuffer[n]*msToSamplesMultiplier;
                        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
                }
                
                float readValue = interShell.interpolate(delayBuffer, readIndex, maxDelayTimeSamples);

                float vn;
                if(inputAR){
                        vn = inputBuffer[n] + readValue * g;
                }else{
                        vn = readValue * g;
                }
                
                
				uint32_t casted = *reinterpret_cast<uint32_t*> ( &(vn) );
				int exponent = casted & 0x7F800000;
				int aNaN = exponent < 0x7F800000;
				int aDen = exponent > 0;
				int result = casted * (aNaN & aDen );
				vn = *reinterpret_cast<float*> ( &result );
                
                delayBuffer[writeIndex] = vn;
                
                outputBuffer[n] = (outGcoeff * readValue) - (g * vn);
                casted = *reinterpret_cast<uint32_t*> ( &(outputBuffer[n]) );
				exponent = casted & 0x7F800000;
				aNaN = exponent < 0x7F800000;
				aDen = exponent > 0;
				result = casted * (aNaN & aDen );
				outputBuffer[n] = *reinterpret_cast<float*> ( &result );       
                
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
