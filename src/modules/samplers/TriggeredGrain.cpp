
#include "TriggeredGrain.h"


pdsp::TriggeredGrain::TriggeredGrain(){ 
    patch(); 
};

pdsp::TriggeredGrain::TriggeredGrain(const TriggeredGrain &other){ 
    patch(); 
};

pdsp::TriggeredGrain& pdsp::TriggeredGrain::operator=(const TriggeredGrain &other){ patch(); return *this; };
    
void pdsp::TriggeredGrain::patch(){

    addModuleInput("position", grain.in_start());
    addModuleInput("length", window.in_length() );
    addModuleInput("pitch", grain.in_pitch() );
    addModuleInput("select", grain.in_select() );
    
    addModuleInput("trig", trigger_in );
    
    addModuleInput("position_jitter", posJitterAmt.in_mod());
    addModuleInput("pitch_jitter", pitchJitterAmt.in_mod());
    addModuleInput("direction", grain.in_direction());        
    addModuleOutput("signal", window);
    addModuleOutput("jitter", jitter);

    grain >> window;
    
    trigger_in >> grain.in_trig();
    trigger_in >> window.in_trig();
    trigger_in >> jitter;
    
    jitter >> posJitterAmt >> grain.in_start();
    jitter >> pitchJitterAmt >> grain.in_pitch();
    

}

float pdsp::TriggeredGrain::meter_env() const{
    return window.meter_window();
}

float pdsp::TriggeredGrain::meter_position() const{
    return grain.meter_position();
}

float pdsp::TriggeredGrain::meter_jitter() const{
    return jitter.meter_output();
}

pdsp::Patchable& pdsp::TriggeredGrain::in_position(){
    return in("position");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_length(){
    return in("length");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_select(){
    return in("select");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_position_jitter(){
    return in("position_jitter");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_pitch_jitter(){
    return in("pitch_jitter");
}

pdsp::Patchable& pdsp::TriggeredGrain::in_direction(){
    return in("direction");
}

pdsp::Patchable& pdsp::TriggeredGrain::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::TriggeredGrain::out_jitter(){
    return out("jitter");
}

void pdsp::TriggeredGrain::addSample(pdsp::SampleBuffer* newSample){
    grain.addSample(newSample);
}

void pdsp::TriggeredGrain::setSample(SampleBuffer* samplePointer, int index){
    grain.setSample(samplePointer, index);
}

void pdsp::TriggeredGrain::setWindowType(Window_t type, int window_length){
    window.setWindowType(type, window_length);
}
