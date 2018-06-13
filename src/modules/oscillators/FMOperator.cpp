
#include "FMOperator.h"


pdsp::FMOperator::FMOperator() { patch(); }
                                        
pdsp::FMOperator::FMOperator(const pdsp::FMOperator &Other){ patch(); }
pdsp::FMOperator& pdsp::FMOperator::operator=(const pdsp::FMOperator &Other){ patch(); return *this; }


void pdsp::FMOperator::patch(){

    addModuleInput( "fm",     phasor.in("pm") );    
    addModuleInput( "pitch",  p2f );    
    addModuleInput( "ratio",  ratioMult.in_mod() );    
    addModuleInput( "fb",     sine.in_shape() );
    addModuleInput( "sync",    phasor.in("sync") );

    addModuleOutput( "signal", sine );
    addModuleOutput( "sync",   phasor.out("sync") );
    
    ratioMult.set(1.0f);
    p2f.enableBoundaries(-30000.0f, 150.0f);
    p2f.set(69.0f); // standard freq is A4 = 440hz
    
    p2f >> ratioMult >> phasor.in_freq() >> sine;
    
}

float pdsp::FMOperator::meter_pitch() const{
    return p2f.meter_input();
}


pdsp::Patchable& pdsp::FMOperator::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::FMOperator::in_fb(){
    return in("fb");
}

pdsp::Patchable& pdsp::FMOperator::in_ratio(){
    return in("ratio");
}

pdsp::Patchable& pdsp::FMOperator::in_fm(){
    return in("fm");
}

pdsp::Patchable& pdsp::FMOperator::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::FMOperator::out_sync(){
    return out("sync");
}

pdsp::Patchable& pdsp::FMOperator::in_sync(){
    return in("sync");
}
