
#include "FMOperator.h"


pdsp::FMOperator::FMOperator() { patch(); }
                                        
pdsp::FMOperator::FMOperator(const pdsp::FMOperator &Other){ patch(); }
pdsp::FMOperator& pdsp::FMOperator::operator=(const pdsp::FMOperator &Other){ patch(); return *this; }


void pdsp::FMOperator::patch(){

    addUnitInput( "fm",     phazor.in("pm") );    
    addUnitInput( "pitch",  p2f );    
    addUnitInput( "ratio",  ratioMult.in_mod() );    
    addUnitInput( "fb",     sine.in_shape() );
    addUnitInput( "sync",    phazor.in("sync") );

    addUnitOutput( "signal", sine );
    addUnitOutput( "sync",   phazor.out("sync") );
    
    ratioMult.set(1.0f);
    
    p2f >> ratioMult >> phazor.in_freq() >> sine;
    
}

float pdsp::FMOperator::meter_pitch(){
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
