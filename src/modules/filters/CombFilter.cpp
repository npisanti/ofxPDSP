
#include "CombFilter.h"

pdsp::CombFilter::CombFilter() { patch(); }
pdsp::CombFilter::CombFilter(const CombFilter& other){ patch(); }
pdsp::CombFilter& pdsp::CombFilter::operator=(const CombFilter& other){ return *this; }


void pdsp::CombFilter::patch(){
    
    addModuleInput( "signal",  delay.in_signal() );
    addModuleInput( "pitch",  p2f );    
    addModuleInput("fb", delay.in_feedback() );
    addModuleInput("damping", delay.in_damping() );
    
    addModuleOutput("signal", delay.out_signal() );

    p2f >> f2ms >> delay.in_time();
    
}
   
pdsp::Patchable& pdsp::CombFilter::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::CombFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::CombFilter::in_fb(){
    return in("fb");
}

pdsp::Patchable& pdsp::CombFilter::in_damping(){
    return in("damping");  
}

pdsp::Patchable& pdsp::CombFilter::out_signal(){
    return out("signal");
}


float pdsp::CombFilter::meter_pitch() const{
    return p2f.meter_input();
}
