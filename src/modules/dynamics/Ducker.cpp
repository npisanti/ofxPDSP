
#include "Ducker.h"

void pdsp::Ducker::patch( ){
    
    channels( 1 );
    
    addModuleInput(  "signal", *amps[0] );
    addModuleOutput( "signal", *amps[0] );    
    
    addModuleInput("trig", envelope.in_trig());
    addModuleInput("velocity", envelope.in_velocity());
    addModuleInput("attack", envelope.in_attack());
    addModuleInput("hold", envelope.in_hold());
    addModuleInput("release", envelope.in_release());
    addModuleInput("ducking", cleanPart);

    cleanPart.enableBoundaries( -3000.0f, 0.0f );
    envelope.set( 50.0f, 0.0f, 100.0f, 0.0f );

    envelope >> oneMinusEnv >> envAmt;
                    ducking >> envAmt.in_mod();
    
    cleanPart >> ducking;
    

}

pdsp::Ducker::~Ducker(){
    channels( 0 );
}
    
void pdsp::Ducker::channels( std::size_t size ){
    std::size_t oldsize = amps.size();
    if( size >= oldsize ){
        amps.resize( size );
        for (std::size_t i=oldsize; i<amps.size(); ++i ){
            amps[i] = new Amp();
            envAmt    >> amps[i]->in_mod();
            cleanPart >> amps[i]->in_mod();
        }        
    }else{
        for( std::size_t i=size; i<oldsize; ++i ){
            delete amps[i];
        }
        amps.resize( size );
    }
}

pdsp::Patchable& pdsp::Ducker::ch( std::size_t index ){
    if( index >= amps.size() ){
        channels(index+1);
    }
    return *(amps[index]);
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

float pdsp::Ducker::meter_env() const {
    return envelope.meter_output();
}

void pdsp::Ducker::setAttackCurve(float hardness) {
    envelope.setAttackCurve(hardness);
}

void pdsp::Ducker::setReleaseCurve(float hardness) {
    envelope.setReleaseCurve(hardness);
}

// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::Ducker::operator[]( std::size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::Ducker::in_0() {
    if( amps.size()<2 ){
        channels(2);
        addModuleInput( "0", *amps[0] );
        addModuleInput( "1", *amps[1] );
        addModuleOutput( "0", *amps[0] );
        addModuleOutput( "1", *amps[1] );
    } 
    return in("0");
}

pdsp::Patchable& pdsp::Ducker::in_1() {
    if( amps.size()<2 ){
        channels(2);
        addModuleInput( "0", *amps[0] );
        addModuleInput( "1", *amps[1] );
        addModuleOutput( "0", *amps[0] );
        addModuleOutput( "1", *amps[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::Ducker::out_0() {
    if( amps.size()<2 ){
        channels(2);
        addModuleInput( "0", *amps[0] );
        addModuleInput( "1", *amps[1] );
        addModuleOutput( "0", *amps[0] );
        addModuleOutput( "1", *amps[1] );
    } 
    return out("0");
}

pdsp::Patchable& pdsp::Ducker::out_1() {
    if( amps.size()<2 ) channels(2);
    return out("1");
}

