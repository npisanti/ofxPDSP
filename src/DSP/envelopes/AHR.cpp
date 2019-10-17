
#include "AHR.h"

pdsp::AHR::AHR(){
        
        stageSwitch = off;
        
        addInput("trig", input_trig);
        addOutput("signal", output);
        addInput("attack", input_attack);
        addInput("hold", input_hold);
        addInput("release", input_release);
        addInput("velocity", input_velocity);        
        updateOutputNodes();
        
        attackTCO = 0.99999;             //digital 
        releaseTCO = exp(-4.95); //digital 
        
        holdLevel = 1.0f;
        
        input_attack.setDefaultValue(0.0f);
        input_hold.setDefaultValue(50.0f);
        input_release.setDefaultValue(50.0f);
        input_velocity.setDefaultValue(1.0f);
        
        dBtrig = false;
        trigcount = 0;
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::AHR::set(float attackTimeMs, float holdTimeMs, float releaseTimeMs, float velocity ){
        input_attack.setDefaultValue(attackTimeMs);
        input_hold.setDefaultValue(holdTimeMs);
        input_release.setDefaultValue(releaseTimeMs);
        input_velocity.setDefaultValue( velocity );
        return *this;
}


void pdsp::AHR::setAttackCurve(float hardness){
    if(hardness < 0.0f) hardness = 0.0f;
    if(hardness > 1.0f) hardness = 1.0f;
    
    float attackTCO = interpolate_linear(EnvelopeStage::digitalAttackTCO, EnvelopeStage::analogAttackTCO, hardness);
    setAttackTCO(attackTCO);
}

void pdsp::AHR::setReleaseCurve(float hardness){
    if(hardness < 0.0f) hardness = 0.0f;
    if(hardness > 1.0f) hardness = 1.0f;
    
    float decayTCO  = interpolate_linear(EnvelopeStage::digitalDecayTCO, EnvelopeStage::analogDecayTCO, hardness);
    setReleaseTCO(decayTCO);
}


void pdsp::AHR::setCurve(float hardness){
    setAttackCurve(hardness);
    setReleaseCurve(hardness);
}
void pdsp::AHR::enableDBTriggering(float dBmin, float dBmax){
    this->dBmin = dBmin;
    this->dBmax = dBmax;
    dBtrig = true;
}

void pdsp::AHR::disableDBTriggering(){
    dBtrig = false;
}


float pdsp::AHR::meter_output() const{
    return meter.load();
}

int pdsp::AHR::meter_triggers() const{
    return trigcount.load();
}

pdsp::Patchable& pdsp::AHR::in_trig(){
    return in("trig");
}
    
pdsp::Patchable& pdsp::AHR::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::AHR::in_hold(){
    return in("hold");
}

pdsp::Patchable& pdsp::AHR::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::AHR::in_velocity(){
    return in("velocity");
}

pdsp::Patchable& pdsp::AHR::out_signal(){
    return out("signal");
}

void pdsp::AHR::prepareUnit( int expectedBufferSize, double sampleRate){
        setEnvelopeSampleRate( sampleRate );
        stageSwitch = off;
        envelopeOutput = 0.0;

        calculateAttackTime();
        calculateHoldTimeAndReset();
        calculateReleaseTime();
}


void pdsp::AHR::releaseResources(){}


void pdsp::AHR::process(int bufferSize) noexcept{
        
        int trigBufferState;
        const float* trigBuffer = processInput(input_trig, trigBufferState);

        switch(trigBufferState){
            case Unchanged: case Changed:
                switch(stageSwitch){
                case off:
                    setOutputToZero(output);
                    meter.store(0.0f);
                    break;
                
                default:
                    process_run(bufferSize);
                    break;
                }
                break;                
           
            case AudioRate:
                process_T(trigBuffer, bufferSize);
                break;
            
            default: break;
        }
}

void pdsp::AHR::onRetrigger(float triggerValue, int n) {
        triggerValue = (triggerValue > 1.0f) ? 1.0f : triggerValue;
        
        if( triggerValue > 0.0f ){
                if(dBtrig){
                    float dB = dBmin*(1.0f - triggerValue) + dBmax*triggerValue;
                    this->intensity = pow(10.0f, dB*0.05f);
                }else{
                    float veloCtrl = processAndGetSingleValue(input_velocity, n);
                    this->intensity = (triggerValue * veloCtrl)  + (1.0f-veloCtrl);                     
                }
                
                stageSwitch = attackStage;
                trigcount++;
        }        

        setAttackTime(  processAndGetSingleValue(input_attack,  n) );
        setHoldTime(    processAndGetSingleValue(input_hold,    n), this->intensity);
        setReleaseTime( processAndGetSingleValue(input_release, n) );
}


void pdsp::AHR::process_run(int bufferSize){

    float* outputBuffer = getOutputBufferToFill(output);
    
    for (int n = 0; n < bufferSize; ++n){
        doEnvelope();
        outputBuffer[n] = envelopeOutput;
    }
    
    meter.store(outputBuffer[0]); 
}


void pdsp::AHR::process_T( const float* &trigBuffer, const int &bufferSize){
        
    float* outputBuffer = getOutputBufferToFill(output);

    for (int n = 0; n < bufferSize; ++n){
        float t = trigBuffer[n];
        if ( notTrigger( t ) ){
            //likely
        }else{ 
            onRetrigger( t, n ); 
        };

        doEnvelope();
        outputBuffer[n] = envelopeOutput;
    }
    
    meter.store(outputBuffer[0]); 
}
