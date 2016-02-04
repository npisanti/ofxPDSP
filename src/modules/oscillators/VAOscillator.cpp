
#include "VAOscillator.h"


pdsp::VAOscillator::VAOscillator() :    saw(pdsp::Hann, true, 4096, true ), 
                                        saw2(pdsp::Hann, true, 4096, true ) { patch(); }
                                        
pdsp::VAOscillator::VAOscillator(const pdsp::VAOscillator &Other){ patch(); }
pdsp::VAOscillator& pdsp::VAOscillator::operator=(const pdsp::VAOscillator &Other){ patch(); return *this; }


void pdsp::VAOscillator::patch(){
    
    addUnitInput( "pitch",  p2f );
    addUnitInput( "pw",  inputShape );
    addUnitInput( "sync",   phazor.in("sync") );
    
    addUnitOutput( "saw",       saw );
    addUnitOutput( "pulse",     leakDC.out_hpf() );
    addUnitOutput( "triangle",  triangle );
    addUnitOutput( "sine",      sine );
    addUnitOutput( "sync",      phazor.out("sync") );
    
    inputShape.enableBoundaries(0.0f, 1.0f);
    inputShape.set(0.5f);
    
    p2f.enableBoundaries(-30000.0f, 150.0f);
    p2f.set(69.0f); // standard freq is A4 = 440hz
    
    p2f >> phazor.in_freq();
    
    phazor >> saw;
    phazor >> triangle;
    phazor >> sine;
    phazor.out("inc") >> saw.in("inc");
    phazor.out("inc") >> triangle.in("inc");
    
    //inputShape * 4.0f >> sine.in_shape();
    
    // makes a pulse wave from two saws
    inputShape >> shift.in_shift();
    phazor >> shift >> saw2;
    phazor.out("inc") >> saw2.in("inc");
    saw  *  0.5f >> leakDC;
    saw2 * -0.5f >> leakDC;
    leakDC.set(20.0f);
    
}

float pdsp::VAOscillator::meter_pitch(){
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
