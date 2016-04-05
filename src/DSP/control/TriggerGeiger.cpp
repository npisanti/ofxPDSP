
#include "TriggerGeiger.h"

pdsp::TriggerGeiger::TriggerGeiger(){
    
    addInput("jitter",   in_jitter_ms );
    addInput("distance", in_distance_ms );
    addOutput("trig", output_trig);
    updateOutputNodes();
    
    in_distance_ms.setDefaultValue(200.0f);
    in_jitter_ms.setDefaultValue(0.0f);

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::TriggerGeiger::set(float distance, float jitter){
    in_distance_ms.setDefaultValue(distance);
    in_jitter_ms.setDefaultValue(jitter); 
    return *this;
}
        
pdsp::Patchable& pdsp::TriggerGeiger::in_distance(){
    return in("distance");
}

pdsp::Patchable& pdsp::TriggerGeiger::in_jitter(){
    return in("jitter");
}

pdsp::Patchable&  pdsp::TriggerGeiger::out_trig(){
    return out("trig");
}

void pdsp::TriggerGeiger::prepareUnit( int expectedBufferSize, double sampleRate ) {
    this->sampleRateMult = sampleRate * 0.001f;
    counter = 0;
}


void pdsp::TriggerGeiger::process (int bufferSize) noexcept {
    
    if(counter < bufferSize){

        int distanceSamples = (processAndGetSingleValue( in_distance_ms, 0 ) * sampleRateMult );
        int jitterSamples = (processAndGetSingleValue( in_jitter_ms, 0 ) * sampleRateMult );

        float* outputBuffer = getOutputBufferToFill(output_trig);
        ofx_Aeq_Zero(outputBuffer, bufferSize);
        
        for(int n=0; n<bufferSize; ++n){
            if(counter==0){
                outputBuffer[n] = 1.0f; //set trigger pulse
                counter = distanceSamples + dice(jitterSamples);
            }
            counter--;
        }

    }else{
        counter -= bufferSize;
        setOutputToZero(output_trig);
    }

}

