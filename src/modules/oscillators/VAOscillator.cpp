
#include "VAOscillator.h"


pdsp::VAOscillator::VAOscillator() :    saw( pdsp::Triangular, false, 1024, true ), 
                                        saw2( pdsp::Triangular, false, 1024, true ) { patch(); }
                                        
pdsp::VAOscillator::VAOscillator(const pdsp::VAOscillator &Other){ patch(); }
pdsp::VAOscillator& pdsp::VAOscillator::operator=(const pdsp::VAOscillator &Other){ patch(); return *this; }


void pdsp::VAOscillator::patch(){
    
    addModuleInput( "pitch",  p2f );
    addModuleInput( "pw",  inputShape );
    addModuleInput( "sync",   phasor.in("sync") );
    
    addModuleOutput( "saw",       saw );
    addModuleOutput( "pulse",     leakDC.out_hpf() );
    addModuleOutput( "triangle",  triangle );
    addModuleOutput( "sine",      sine );
    addModuleOutput( "sync",      phasor.out("sync") );
    
    inputShape.enableBoundaries(0.0f, 1.0f);
    inputShape.set(0.5f);
    
    p2f.enableBoundaries(-30000.0f, 150.0f);
    p2f.set(69.0f); // standard freq is A4 = 440hz
    
    p2f >> phasor.in_freq();
    
    phasor >> saw;
    phasor >> triangle;
    phasor >> sine;
    phasor.out("inc") >> saw.in("inc");
    
    // makes a pulse wave from two saws
    inputShape >> shift.in_shift();
    phasor >> shift >> saw2;
    phasor.out("inc") >> saw2.in("inc");
    saw  *  0.5f >> leakDC;
    saw2 * -0.5f >> leakDC;
    leakDC.set(20.0f);
    
}

float pdsp::VAOscillator::meter_pitch() const{
    return p2f.meter_input();
}


pdsp::Patchable& pdsp::VAOscillator::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::VAOscillator::in_pw(){
    return in("pw");
}

pdsp::Patchable& pdsp::VAOscillator::in_sync(){
    return in("sync");
}

pdsp::Patchable& pdsp::VAOscillator::out_saw(){
    return out("saw");
}

pdsp::Patchable& pdsp::VAOscillator::out_pulse(){
    return out("pulse");
}

pdsp::Patchable& pdsp::VAOscillator::out_triangle(){
    return out("triangle");
}

pdsp::Patchable& pdsp::VAOscillator::out_sine(){
    return out("sine");
}

pdsp::Patchable& pdsp::VAOscillator::out_sync(){
    return out("sync");
}
