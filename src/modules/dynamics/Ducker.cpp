
#include "Ducker.h"

void pdsp::Ducker::patch( ){
    
    addModuleInput("0", input1);
    addModuleInput("1", input2);
    
    addModuleInput("trig", envelope.in_trig());
    addModuleInput("velocity", envelope.in_velocity());
    addModuleInput("attack", envelope.in_attack());
    addModuleInput("hold", envelope.in_hold());
    addModuleInput("release", envelope.in_release());
    addModuleInput("ducking", cleanPart);
    
    addModuleOutput("0", dca1);
    addModuleOutput("1", dca2);
    
    cleanPart.enableBoundaries( -3000.0f, 0.0f );
    envelope.set( 50.0f, 0.0f, 100.0f, 0.0f );

    input1 >> dca1;
    input2 >> dca2;
    
    envelope >> oneMinusEnv >> envAmt;
                    ducking >> envAmt.in_mod();
    
    cleanPart >> ducking;
    
    envAmt    >> dca1.in_mod();
    envAmt    >> dca2.in_mod();
    cleanPart >> dca1.in_mod();
    cleanPart >> dca2.in_mod();
}

pdsp::Patchable& pdsp::Ducker::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::Ducker::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::Ducker::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::Ducker::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::Ducker::in_velocity(){
    return in("velocity");
}

pdsp::Patchable& pdsp::Ducker::in_hold(){
    return in("hold");
}

pdsp::Patchable& pdsp::Ducker::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::Ducker::in_ducking(){
    return in("ducking");
}

pdsp::Patchable& pdsp::Ducker::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::Ducker::out_1(){
    return out("1");
}

float pdsp::Ducker::meter_env() const {
    return envelope.meter_output();
}

void pdsp::Ducker::setAttackCurve(float hardness) {
    envelope.setAttackCurve(hardness);
}

void pdsp::Ducker::setReleaseCurve(float hardness) {
    envelope.setReleaseCurve(hardness);
}

