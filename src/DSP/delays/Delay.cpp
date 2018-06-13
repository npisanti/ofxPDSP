

#include "Delay.h"


pdsp::Delay::Delay() : Delay(1000.0f){};

                
pdsp::Delay::Delay( float timeMs){
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
        
        input_damping.enableBoundaries( 0.0f, 0.99f);
        
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

pdsp::Patchable& pdsp::Delay::set( float timeMs, float feedback, float damping){
    in_time_ms.setDefaultValue(timeMs);
    input_feedback.setDefaultValue(feedback);
    input_damping.setDefaultValue(damping);
    return *this;
}

float pdsp::Delay::meter_time() const{
    return timeMeter.load();
}

pdsp::Patchable& pdsp::Delay::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Delay::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::Delay::in_time(){
    return in("time");
}

pdsp::Patchable& pdsp::Delay::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::Delay::in_damping(){
    return in("damping");
}

void pdsp::Delay::changeInterpolator(Interpolator_t interpolatorMode){
        interShell.changeInterpolator(interpolatorMode);
}
/*
void pdsp::Delay::timeBoundaryEnabled(bool enable){
        this->boundaries = enable;
                
        updateBoundaries();
}
*/

void pdsp::Delay::updateBoundaries(){
        
        if(boundaries){
                float low = 3000.0f / this->sampleRate;
                float high = maxDelayTimeMs - (1000.0f/this->sampleRate);
                in_time_ms.enableBoundaries( low, high);
        }else{
                in_time_ms.disableBoundaries();
        }

}

void pdsp::Delay::setDamping(float hiDamp){        
        g = hiDamp;
        if (g >= 1.0){
                g = 0.99;
        }
        gLPF = g * (1.0f - feedback);

}

void pdsp::Delay::setMaxTime(float timeMs){
        maxDelayTimeMs = timeMs;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }	
}


void pdsp::Delay::setFeedback(float feedback){ 
        this->feedback = feedback; 
        setDamping(g);
};


void pdsp::Delay::prepareUnit ( int expectedBufferSize, double sampleRate ){
        z1 = 0.0f;
        this->sampleRate = sampleRate;
        
        msToSamplesMultiplier = this->sampleRate * 0.001;
        
        maxDelayTimeSamples = static_cast<int> (maxDelayTimeMs * msToSamplesMultiplier) +1; //1 is the guard point
        maxDelayTimeSamples_f = static_cast<float>(maxDelayTimeSamples);

        initDelayBuffer();
        updateBoundaries();

}

void pdsp::Delay::releaseResources(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
}

void pdsp::Delay::initDelayBuffer(){
        if (delayBuffer != nullptr){
                ofx_deallocate_aligned(delayBuffer);
        }
        ofx_allocate_aligned(delayBuffer, maxDelayTimeSamples);
        for (int i = 0; i < maxDelayTimeSamples; ++i){
                delayBuffer[i] = 0.0f;
        }
}


void pdsp::Delay::process(int bufferSize) noexcept{
        
        int fbBufferState;
        const float* fbBuffer = processInput(input_feedback, fbBufferState);
        
        int dampBufferState;
        const float* dampBuffer = processInput(input_damping, dampBufferState);

        int inputBufferState;
        const float* inputBuffer = processInput( input, inputBufferState );

        int timeBufferState;
        const float* timeBuffer = processInput( in_time_ms, timeBufferState );

        
        int switcherOnce = timeBufferState + fbBufferState*4 + dampBufferState*16;
        
        switch ( switcherOnce & processOnceBitMask ) {
        case onceFFFF:
                process_once<false, false, false> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceTFFF:
                process_once<true, false, false> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceFTFF:
                process_once<false, true, false> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceTTFF:
                process_once<true, true, false> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceFFTF:
                process_once<false, false, true> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceTFTF:
                process_once<true, false, true> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceFTTF:
                process_once<false, true, true> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        case onceTTTF:
                process_once<true, true, true> ( timeBuffer, fbBuffer, dampBuffer);
                break;
        }        


        int switcherAudio = inputBufferState + switcherOnce*4;

        switch ( switcherAudio & processAudioBitMask ) {
        case audioFFFF:
                process_audio<false, false, false, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTFFF: 
                process_audio<true, false, false, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer,bufferSize);
                break;
        case audioFTFF:
                process_audio<false, true, false, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTTFF:
                process_audio<true, true, false, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
      
        case audioFFTF:
                process_audio<false, false, true, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTFTF:
                process_audio<true, false, true, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioFTTF:
                process_audio<false, true, true, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTTTF:
                process_audio<true, true, true, false> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
      
        case audioFFFT:
                process_audio<false, false, false, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTFFT:
                process_audio<true, false, false, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioFTFT:
                process_audio<false, true, false, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTTFT:
                process_audio<true, true, false, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
    
        case audioFFTT:
                process_audio<false, false, true, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTFTT:
                process_audio<true, false, true, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioFTTT:
                process_audio<false, true, true, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        case audioTTTT:
                process_audio<true, true, true, true> (inputBuffer, timeBuffer, fbBuffer, dampBuffer, bufferSize);
                break;
        }
        
        timeMeter.store( timeBuffer[0] );
}



template<bool timeChange, bool fbChange, bool dampChange>    
void pdsp::Delay::process_once(const float* timeBuffer, const float* fbBuffer, const float* dampBuffer )noexcept {
    
        if( timeChange ){
            readIndex = static_cast<float>(writeIndex) - timeBuffer[0]*msToSamplesMultiplier;
            if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
        }
        
        if(fbChange){
            feedback = fbBuffer[0];
            
        }
        
        if(dampChange){
            g = dampBuffer[0];
        }
        
        if(fbChange || dampChange){
            gLPF = g * (1.0f - feedback);
        }
}


template<bool inputAR, bool timeAR, bool fbAR, bool dampAR>
void pdsp::Delay::process_audio(const float* inputBuffer, const float* timeBuffer, const float* fbBuffer, const float* dampBuffer, int bufferSize )noexcept{

        float* outputBuffer = getOutputBufferToFill( output );

        for(int n=0; n<bufferSize; ++n){
                if(timeAR){
                        readIndex = static_cast<float>(writeIndex) - timeBuffer[n]*msToSamplesMultiplier;
                        if (readIndex < 0) readIndex = maxDelayTimeSamples + readIndex;
                }
                
                int index_int = static_cast<int> (readIndex);
                float mu = readIndex - index_int;
                float x1 = delayBuffer[index_int];
                float x2 = delayBuffer[index_int+1];

                outputBuffer[n] = (x1 * (1.0f-mu)) + (x2 * mu);
                
                if(fbAR){
                    feedback = fbBuffer[n];
                }
                if(dampAR){
                    g = dampBuffer[n];
                }                
                if(fbAR || dampAR){
                    gLPF = g * (1.0f - feedback);
                }            
                    
                //filter code
                float readValue = (outputBuffer[n] + gLPF * z1);
                z1 = readValue;
                //end filter code
                
                if(inputAR){
                        delayBuffer[writeIndex] = inputBuffer[n] + readValue * feedback;
                }else{
                        delayBuffer[writeIndex] = readValue * feedback;
                }

				uint32_t casted = *reinterpret_cast<uint32_t*> ( &(delayBuffer[writeIndex]) );
				int exponent = casted & 0x7F800000;
				int aNaN = exponent < 0x7F800000;
				int aDen = exponent > 0;
				int result = casted * (aNaN & aDen );
				delayBuffer[writeIndex] = *reinterpret_cast<float*> ( &result );

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
