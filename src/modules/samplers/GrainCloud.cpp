
#include "GrainCloud.h"

pdsp::GrainCloud::MultiGrainTrigger::MultiGrainTrigger(int outputs){
    this->outputs = outputs;
    
    addInput("jitter",   in_jitter_ms );
    addInput("distance", in_distance_ms );
    
    outs.resize(outputs);
    
    for(int i=0; i<outputs; ++i){
        addOutput( std::to_string(i).c_str(), outs[i] );
    }
    
    updateOutputNodes();
    
    in_distance_ms.setDefaultValue(200.0f);
    in_jitter_ms.setDefaultValue(0.0f);

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

void pdsp::GrainCloud::MultiGrainTrigger::prepareUnit( int expectedBufferSize, double sampleRate ) {
    this->sampleRateMult = sampleRate * 0.001f;
    counter = 0;
    currentOut = 0;
}

void pdsp::GrainCloud::MultiGrainTrigger::process (int bufferSize) noexcept {
    
    if(counter < bufferSize){
        
        int distanceSamples = (processAndGetSingleValue( in_distance_ms, 0 ) * sampleRateMult );
        int jitterSamples = (processAndGetSingleValue( in_jitter_ms, 0 ) * sampleRateMult );

        for(int i=0; i<outputs; ++i){
            setOutputToZero(outs[i]);
        }
        
        for(int n=0; n<bufferSize; ++n){
            if(counter==0){
                float* outputBuffer = getOutputBufferToFill(outs[currentOut]);
                ofx_Aeq_Zero(outputBuffer, bufferSize);
                outputBuffer[n] = 1.0f; //set trigger pulse
                counter = distanceSamples + dice(jitterSamples);
                
                currentOut++;
                if (currentOut==outputs) currentOut = 0;
            }
            counter--;
        }

    }else{
        
        counter -= bufferSize;
        for(int i=0; i<outputs; ++i){
            setOutputToZero(outs[i]);
        }
        
    }

}


pdsp::GrainCloud::GrainCloud() : GrainCloud(8) {};

pdsp::GrainCloud::GrainCloud(int voices) : triggers(voices) { 
    this->voices = voices;
    patch(); 
};

pdsp::GrainCloud::GrainCloud(const GrainCloud &other) : GrainCloud(1) { 
    std::cout<<"[pdsp] warning: copy used on module (GrainCloud), undefined behavior\n";
};

pdsp::GrainCloud& pdsp::GrainCloud::operator=(const GrainCloud &other){ 
    std::cout<<"[pdsp] warning: move operator used on module (GrainCloud), undefined behavior\n";
    return *this; 
};
    
void pdsp::GrainCloud::patch(){

    addModuleInput("position", ctrl_start );
    addModuleInput("length", ctrl_grain_length );
    addModuleInput("pitch", ctrl_pitch );
    addModuleInput("select", ctrl_select );
    
    addModuleInput("density", ctrl_density );
    addModuleInput("distance_jitter", triggers.in("jitter") );
    
    addModuleInput("position_jitter", ctrl_pos_jit );
    addModuleInput("pitch_jitter", ctrl_pitch_jit );
    addModuleInput("direction", ctrl_direction );      
      
    addModuleOutput("L", outL);
    addModuleOutput("R", outR);

    streams.resize(voices);
        
    float scale = (1.0f/static_cast<float>(voices));
    
    ctrl_select.set(0.0f);
    ctrl_pos_jit * 0.5f >> ctrl_pos_jit_scale;
    
    for(int i=0; i<voices; ++i){
        ctrl_start >> streams[i].in_position();
        ctrl_grain_length >> streams[i].in_length();
        ctrl_pitch >> streams[i].in_pitch();
        ctrl_select >> streams[i].in_select();
        ctrl_direction >> streams[i].in_direction();
        ctrl_pitch_jit >> streams[i].in_pitch_jitter();
        ctrl_pos_jit_scale >> streams[i].in_position_jitter();
        
        triggers.outs[i] >> streams[i].in_trig();
        
        if(i%2==0){            
            streams[i] >> outL.set(scale*2.0f);
        }else{
            streams[i] >> outR.set(scale*2.0f);
        }
    }

    ctrl_grain_length >> density_amp;
    scale >> density_amp.in_mod();
    ctrl_density >> density_amp.in_mod();
    ctrl_density.enableBoundaries(0.0f, 1.0f);
    density_amp >> triggers.in("distance");

    setWindowType(Tukey, 1024);
    //ctr_grain_length * (1.0f/static_cast<float>(streams)) >> triggers.in("distance");
    //distance could became "density"
}


float pdsp::GrainCloud::meter_env(int voice) const {
    return streams[voice].meter_env();
}

float pdsp::GrainCloud::meter_position(int voice) const{
    return streams[voice].meter_position();
}

float pdsp::GrainCloud::meter_jitter(int voice) const{
    return streams[voice].meter_jitter();
}

pdsp::Patchable& pdsp::GrainCloud::in_position(){
    return in("position");
}

pdsp::Patchable& pdsp::GrainCloud::in_length(){
    return in("length");
}

pdsp::Patchable& pdsp::GrainCloud::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::GrainCloud::in_select(){
    return in("select");
}

pdsp::Patchable& pdsp::GrainCloud::in_density(){
    return in("density");
}

pdsp::Patchable& pdsp::GrainCloud::in_distance_jitter(){
    return in("distance_jitter");
}

pdsp::Patchable& pdsp::GrainCloud::in_position_jitter(){
    return in("position_jitter");
}

pdsp::Patchable& pdsp::GrainCloud::in_pitch_jitter(){
    return in("pitch_jitter");
}

pdsp::Patchable& pdsp::GrainCloud::in_direction(){
    return in("direction");
}

pdsp::Patchable& pdsp::GrainCloud::out_L(){
    return out("L");
}

pdsp::Patchable& pdsp::GrainCloud::out_R(){
    return out("R");
}

void pdsp::GrainCloud::setSample(SampleBuffer* samplePointer, int index){
    for(int i=0; i<voices; ++i){
        streams[i].setSample(samplePointer, index);
    }
}

void pdsp::GrainCloud::addSample(SampleBuffer* samplePointer){
    for(int i=0; i<voices; ++i){
        streams[i].addSample(samplePointer);
    }
}

void pdsp::GrainCloud::setWindowType(Window_t type, int window_length){
    for(int i=0; i<voices; ++i){
        streams[i].setWindowType(type, window_length);
    }
}

void pdsp::GrainCloud::setInterpolatorType(Interpolator_t type){
    for(int i=0; i<voices; ++i){
        streams[i].setInterpolatorType(type);
    }
} 

int pdsp::GrainCloud::getVoicesNum() const {
    return voices;
}
