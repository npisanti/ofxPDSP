
#include "ClockedLFO.h"


//TODO: DIVISION TO FREQ FORMULA, actually noise is broken in Clocked mode


pdsp::ClockedLFO::ClockedLFO() { patch(); }
pdsp::ClockedLFO::ClockedLFO(const pdsp::ClockedLFO &Other){ patch(); }
pdsp::ClockedLFO& pdsp::ClockedLFO::operator=(const pdsp::ClockedLFO &Other){ return *this; }


void pdsp::ClockedLFO::patch(){
    
    addModuleInput( "division",   phazorClocked.in_division() );
    addModuleInput( "retrig",  phazorClocked.in_retrig() );    
    addModuleInput( "phase_offset",  phazorClocked.in_phase_offset() );  

    addModuleOutput( "triangle", triangle );
    addModuleOutput( "sine", sine );
    addModuleOutput( "saw", sawSlew );
    addModuleOutput( "square", squareSlew );
    addModuleOutput( "sample_and_hold", randomSnHSlew );
    

    phazorClocked  >> saw      >> sawSlew;
    phazorClocked  >> square   >> squareSlew;
    phazorClocked  >> sine;
    phazorClocked  >> triangle;
    phazorClocked.out_trig() >> rnd >> randomSnHSlew;   
        
    sawSlew.set(200.0f); //200hz slew = 5ms
    squareSlew.set(200.0f); //200hz slew = 5ms
    randomSnHSlew.set(200.0f); //200hz slew = 5ms

}

pdsp::Patchable& pdsp::ClockedLFO::set(float division, float phaseOffset){
    phazorClocked.set(division, phaseOffset);
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
