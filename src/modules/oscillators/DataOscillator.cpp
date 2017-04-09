
#include "DataOscillator.h"

pdsp::DataOscillator::DataOscillator() { patch(); }
                                        
pdsp::DataOscillator::DataOscillator(const pdsp::DataOscillator &Other){ patch(); }
pdsp::DataOscillator& pdsp::DataOscillator::operator=(const pdsp::DataOscillator &Other){ patch(); return *this; }

void pdsp::DataOscillator::patch(){

    addModuleInput( "pitch",  p2f );        
    addModuleOutput( "signal", wto );
    
    p2f.enableBoundaries(-30000.0f, 150.0f);
    p2f.set(69.0f); // standard freq is A4 = 440hz
    
    p2f >> phazor.in_freq() >> wto;
    
}

void pdsp::DataOscillator::setTable(DataTable& dataTable) {
    wto.setTable( dataTable );
}

float pdsp::DataOscillator::meter_pitch() const{
    return p2f.meter_input();
}


pdsp::Patchable& pdsp::DataOscillator::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::DataOscillator::out_signal(){
    return out("signal");
}
