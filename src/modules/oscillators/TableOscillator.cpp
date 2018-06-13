
#include "TableOscillator.h"

pdsp::TableOscillator::TableOscillator() { patch(); }
                                        
pdsp::TableOscillator::TableOscillator(const pdsp::TableOscillator &Other){ patch(); }
pdsp::TableOscillator& pdsp::TableOscillator::operator=(const pdsp::TableOscillator &Other){ patch(); return *this; }

void pdsp::TableOscillator::patch(){

    addModuleInput( "pitch",  p2f );    
    addModuleInput( "table",  wto.in_shape() );    
    addModuleOutput( "signal", wto );
    
    p2f.enableBoundaries(-30000.0f, 150.0f);
    p2f.set(69.0f); // standard freq is A4 = 440hz
    
    p2f >> phasor.in_freq() >> wto;
    
}

void pdsp::TableOscillator::setTable(WaveTable& waveTable) {
    wto.setTable( waveTable );
}

float pdsp::TableOscillator::meter_pitch() const{
    return p2f.meter_input();
}


pdsp::Patchable& pdsp::TableOscillator::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::TableOscillator::in_table(){
    return in("table");
}

pdsp::Patchable& pdsp::TableOscillator::out_signal(){
    return out("signal");
}



