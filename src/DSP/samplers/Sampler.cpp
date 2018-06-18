
#include "Sampler.h"

pdsp::Sampler::Sampler(){

        addInput("trig", input_trig);
        addInput("pitch", input_pitch_mod);
        addInput("select", input_select);
        addInput("start", input_start);
        addInput("direction", input_direction);
        addInput("start_mod", input_start_mod);
        addOutput("signal", output);
        updateOutputNodes();  
        
        sampleIndex = 0;
        incBase = 1.0f / 11050.0f;
        sample = nullptr;
        readIndex = 0.0f;
        direction = 1.0f;
        
        positionMeter.store(0.0f);
        positionDivider = 0.001f;

        input_pitch_mod.setDefaultValue(0.0f);
        input_select.setDefaultValue(0.0f);
        input_start.setDefaultValue(0.0f);
        input_start_mod.setDefaultValue(0.0f);
        input_direction.setDefaultValue(1.0f);

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

float pdsp::Sampler::meter_position() const{
    return positionMeter.load();
}

pdsp::Patchable& pdsp::Sampler::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::Sampler::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::Sampler::in_select(){
    return in("select");
}

pdsp::Patchable& pdsp::Sampler::in_start(){
    return in("start");
}

pdsp::Patchable& pdsp::Sampler::in_start_mod(){
    return in("start_mod");
}

pdsp::Patchable& pdsp::Sampler::in_direction(){
    return in("direction");
}

pdsp::Patchable& pdsp::Sampler::out_signal(){
    return out("signal");
}


void pdsp::Sampler::addSample(SampleBuffer* newSample, int channel){
        samples.push_back(newSample);
        channels.push_back(channel);
        if(sample==nullptr){
                sample = newSample;
        }  
}

bool pdsp::Sampler::setSample(SampleBuffer* newSample, int index, int channel){
        if(index<samples.size() && index>=0){
                samples[index] = newSample;
                channels[index] = channel;
                return true;
        }if(index == samples.size() ){
                addSample(newSample, channel);
                return true;
        }else{
                return false;
                std::cout<< "[pdsp] warning! sample not set, wrong index given!\n";
                pdsp_trace();
        }
}

void pdsp::Sampler::prepareUnit( int expectedBufferSize, double sampleRate ) {
        readIndex = 0.0f;
        incBase = 1.0f / sampleRate;
}



void pdsp::Sampler::process(int bufferSize) noexcept {
        
        if(sample!=nullptr && sample->loaded()){
        
                selectState = Unchanged;
                selectBuffer = nullptr;

                startState = Unchanged;
                startBuffer = nullptr;
                
                startModState = Unchanged;
                startModBuffer = nullptr;

                int triggerState;
                const float* triggerBuffer = processInput( input_trig, triggerState);

                int pitchModState;
                const float* pitchModBuffer = processInput( input_pitch_mod, pitchModState  );

            
                if(pitchModState!=AudioRate){
                        process_once<true>(pitchModBuffer);
                }


                int switcher = pitchModState + triggerState*4;
                switch ( switcher & processAudioBitMask ) {
                case audioFFFF : //false, false
                        process_audio<false, false>(pitchModBuffer, triggerBuffer, bufferSize);
                        break;
                case audioTFFF : //true, false
                        process_audio<true, false>(pitchModBuffer, triggerBuffer, bufferSize);
                        break;
                case audioFTFF : //false, true
                        process_audio<false, true>(pitchModBuffer, triggerBuffer, bufferSize);
                        break;
                case audioTTFF : //true, true
                        process_audio<true, true>(pitchModBuffer, triggerBuffer, bufferSize);
                        break;
                default:
                        break;
                }
        }else{
                setOutputToZero(output);
        }

}



template<bool pitchModChange>
void pdsp::Sampler::process_once( const float* pitchModBuffer)noexcept{
        if(pitchModChange){
                vect_calculateIncrement(&inc, pitchModBuffer, incBase * sample->fileSampleRate, 1);
                //in this way is always correct even with oversample
        }
}

template<bool pitchModAR, bool triggerAR>
void pdsp::Sampler::process_audio( const float* pitchModBuffer, const float* triggerBuffer, int bufferSize)noexcept{

        float* outputBuffer = getOutputBufferToFill(output);
        
        sample = samples[sampleIndex]; // this will make hot-swap of SampleBuffer files more robust
        channel = channels[sampleIndex]; 

        if(pitchModAR){
                //
                vect_calculateIncrement(outputBuffer, pitchModBuffer, incBase * sample->fileSampleRate, bufferSize);
                //in this way is always correct even with oversample
        }

        for(int n=0; n<bufferSize; ++n){

                if(triggerAR){
                        if(checkTrigger(triggerBuffer[n])){
                                selectSample( n, bufferSize, triggerBuffer[n] );
                        }
                }

                if(pitchModAR){
                        inc = outputBuffer[n];  //we have the calculated pitchs inside outputbuffer
                }

                int readIndex_int = static_cast<int>(readIndex);
                if(readIndex_int>=0 && readIndex_int < sample->length){
                    
                        int index_int = static_cast<int> (readIndex);
                        float mu = readIndex - index_int;
                        float x1 = sample->buffer[channel][index_int];
                        float x2 = sample->buffer[channel][index_int+1];

                        outputBuffer[n] = interpolate_smooth( x1, x2, mu );
                }else{
                        outputBuffer[n] = 0.0f;
                }
                
                readIndex += inc*direction;
                 
        }
        
        positionMeter.store(readIndex*positionDivider);
}


void pdsp::Sampler::selectSample( int n, int bufferSize, float trigger )noexcept{

        if(selectBuffer==nullptr){
                selectBuffer = processInput(input_select, selectState);
        }

        if(startBuffer==nullptr){
                startBuffer = processInput(input_start, startState);
        }
        
        if(startModBuffer==nullptr){
                startModBuffer = processInput(input_start_mod, startModState);
        }

        //SELECT SAMPLE

        if (selectState==AudioRate){
                sampleIndex = static_cast<int>(selectBuffer[n]);
        }else{
                sampleIndex = static_cast<int>(selectBuffer[0]);
        }
        
        if(sampleIndex<0){
                sampleIndex=0;
        }else if(sampleIndex>=samples.size()){
                sampleIndex = samples.size() - 1 ;
        }
        
        //SET START POSITION
        sample = samples[sampleIndex];
        channel = channels[sampleIndex];
        
        trigger = (trigger > 1.0f) ? 1.0f : trigger;
        trigger = (trigger < 0.0f) ? 0.0f : trigger;
        
        float startAdd = (1.0f-trigger) * startModBuffer[0];
        
        float start;
        if(startState==AudioRate){
                start = startBuffer[n];
        }else{
                start = startBuffer[0];
        }
        
        start += startAdd;
        
        start = (start > 1.0f) ? 1.0f : start;
        start = (start < 0.0f) ? 0.0f : start;
        
        float lenFloat = static_cast<float>(sample->length);
        readIndex = start * lenFloat;
        positionDivider = 1.0f / lenFloat;

        //SET FORWARD OR REVERSE
        float directionControl = processAndGetSingleValue( input_direction, n ); 
        if(directionControl<0.0f){
            direction = -1.0f;
        }else{
            direction = 1.0f;
        }

}
