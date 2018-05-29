
#include "EnvelopeFollower.h"

pdsp::EnvelopeFollower::EnvelopeFollower(){
    addInput("signal", input);
    addInput("attack", in_attack_ms);
    addInput("release", in_release_ms);
    addOutput("signal", output);
    updateOutputNodes();
    
    //TC = logf(0.368); //Analog
    TC = logf(0.01); //Digital
    envelopeOutput = 0.0f;
    in_attack_ms.setDefaultValue(20.0f);
    in_release_ms.setDefaultValue(50.0f);
    
    meter.store(0.0f);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

void pdsp::EnvelopeFollower::analog(){
    TC = logf(0.368); //Analog
}

void pdsp::EnvelopeFollower::digital(){
    TC = logf(0.01); //Digital
}

pdsp::Patchable& pdsp::EnvelopeFollower::set(float attack_ms, float release_ms){
    in_attack_ms.setDefaultValue(attack_ms);
    in_release_ms.setDefaultValue(release_ms);
    return *this;
}

float pdsp::EnvelopeFollower::meter_output() const{
    return meter.load();
}

pdsp::Patchable& pdsp::EnvelopeFollower::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::EnvelopeFollower::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::EnvelopeFollower::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::EnvelopeFollower::out_signal(){
    return out("signal");
}

void pdsp::EnvelopeFollower::prepareUnit( int expectedBufferSize, double sampleRate ){
    this->sampleRate = sampleRate;
    envelopeOutput = 0.0f;
    attackCoeff  = expf( TC / (50.0f * sampleRate * 0.001f) );
    releaseCoeff = expf( TC / (50.0f * sampleRate * 0.001f) );
}

void pdsp::EnvelopeFollower::releaseResources () {
    
}

void pdsp::EnvelopeFollower::process (int bufferSize) noexcept {
    
    int inputState;
    const float* inputBuffer = processInput(input, inputState);

    if(inputState==AudioRate){
                           
        int changed;
        const float* attackBuffer = processInput(in_attack_ms, changed);
        if(changed) {  attackCoeff  = expf( TC / (attackBuffer[0]  * sampleRate * 0.001f) ); }
        const float* releaseBuffer = processInput(in_release_ms, changed);
        if(changed) {  releaseCoeff = expf( TC / (releaseBuffer[0] * sampleRate * 0.001f) ); }
        
        float* outputBuffer = getOutputBufferToFill(output);
        
        for(int n=0; n<bufferSize; ++n){
            float xn = inputBuffer[n]; 
            if(xn > envelopeOutput ){
                envelopeOutput = attackCoeff  * (envelopeOutput-xn) + xn;
            }else{
                envelopeOutput = releaseCoeff * (envelopeOutput-xn) + xn;
            }
            outputBuffer[n] = envelopeOutput;
        }
        
        meter.store(outputBuffer[0]);
        
    }else{
    
        setOutputToZero( output );
        meter.store( 0.0f );
        
    }
    
}


