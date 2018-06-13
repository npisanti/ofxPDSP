
#include "ClockedLFO.h"


pdsp::ClockedLFO::ClockedLFO() { patch(); }
pdsp::ClockedLFO::ClockedLFO(const pdsp::ClockedLFO &Other){ patch(); }
pdsp::ClockedLFO& pdsp::ClockedLFO::operator=(const pdsp::ClockedLFO &Other){ return *this; }


void pdsp::ClockedLFO::patch(){
    
    addModuleInput( "division",   phasorClocked.in_division() );
    addModuleInput( "retrig",  phasorClocked.in_retrig() );    
    addModuleInput( "phase_offset",  phasorClocked.in_phase_offset() );  

    addModuleOutput( "triangle", triangle );
    addModuleOutput( "sine", sine );
    addModuleOutput( "saw", sawSlew );
    addModuleOutput( "square", squareSlew );
    addModuleOutput( "sample_and_hold", randomSnHSlew );
    

    phasorClocked  >> saw      >> sawSlew;
    phasorClocked  >> square   >> squareSlew;
    phasorClocked  >> sine;
    phasorClocked  >> triangle;
    phasorClocked.out_trig() >> rnd >> randomSnHSlew;   
        
    sawSlew.set(200.0f);
    squareSlew.set(200.0f);
    randomSnHSlew.set(200.0f);

}

pdsp::Patchable& pdsp::ClockedLFO::set(float division, float phaseOffset){
    phasorClocked.set(division, phaseOffset);
    return *this;
}

pdsp::Patchable& pdsp::ClockedLFO::in_division(){
    return in("division");
}

pdsp::Patchable& pdsp::ClockedLFO::in_phase_offset(){
    return in("phase_offset");
}  

pdsp::Patchable& pdsp::ClockedLFO::in_retrig(){
    return in("retrig");
}

pdsp::Patchable& pdsp::ClockedLFO::out_triangle(){
    return out("triangle");
}

pdsp::Patchable& pdsp::ClockedLFO::out_sine(){
    return out("sine");
}

pdsp::Patchable& pdsp::ClockedLFO::out_saw(){
    return out("saw");
}

pdsp::Patchable& pdsp::ClockedLFO::out_square(){
    return out("square");
}

pdsp::Patchable& pdsp::ClockedLFO::out_sample_and_hold(){
    return out("sample_and_hold");
}
