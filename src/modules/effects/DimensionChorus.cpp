
#include "DimensionChorus.h"

pdsp::DimensionChorus::DimensionChorus() { patch(); }
pdsp::DimensionChorus::DimensionChorus(const DimensionChorus& other){ patch(); }
pdsp::DimensionChorus& pdsp::DimensionChorus::operator=(const DimensionChorus& other){ return *this; }

pdsp::DimensionChorus::Channel::Channel(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void pdsp::DimensionChorus::patch(){
    
    addModuleInput( "L",  channel0.input );
    addModuleInput( "R",  channel1.input );    
    addModuleOutput("L", channel0.output);
    addModuleOutput("R", channel1.output);
    
    addModuleInput("speed", speed);
    addModuleInput("depth", depth);
    addModuleInput("delay", delay);
    
    speed.set(0.25);
    //speed.enableBoundaries(0.15f, 0.6f);
    depth.set(3.5f);
    //depth.enableBoundaries(0.0f, 30.0f);
    delay.set(9.0f);
    //delay.enableBoundaries(1.0f, 100.0f);


                       channel0.input >>           channel0.output;
                       channel0.input >> delay1 >> channel0.output;
    channel1.input * -1.0f >> filter2.out_hpf() >> channel0.output;
    
                       channel1.input >>           channel1.output;
                       channel1.input >> delay2 >> channel1.output;
    channel0.input * -1.0f >> filter1.out_hpf() >> channel1.output;
                    
        
    speed >> phasor.in_freq() >> LFO >> mod1  >> delay1.in_time();
                                        delay >> delay1.in_time();
                                 LFO >> mod2  >> delay2.in_time();
                                        delay >> delay2.in_time();
                                        
    depth         >> mod1.in_mod();
    depth * -1.0f >> mod2.in_mod();
                         
    filter1.set(500.0f); // set default cutoff
    filter2.set(500.0f); // set default cutoff                              
}

pdsp::Patchable& pdsp::DimensionChorus::ch( size_t index ){
    wrapChannelIndex( index, 2, "pdsp::DimensionChorus" );
    
    switch( index ){
        case 0: return channel0; break;
        case 1: return channel1; break;
    }
    
    return channel0;
}

pdsp::Patchable& pdsp::DimensionChorus::in_speed(){
    return in("speed");
}

pdsp::Patchable& pdsp::DimensionChorus::in_depth(){
    return in("depth");
}

pdsp::Patchable& pdsp::DimensionChorus::in_delay(){
    return in("delay");
}

float pdsp::DimensionChorus::meter_lfo() const{
    return LFO.meter_output();
}


// ------------------ backward compatibility ------------------------

pdsp::Patchable& pdsp::DimensionChorus::in_L(){
    return in("L");
}

pdsp::Patchable& pdsp::DimensionChorus::in_R(){
    return in("R");
}

pdsp::Patchable& pdsp::DimensionChorus::out_L(){
    return out("L");
}

pdsp::Patchable& pdsp::DimensionChorus::out_R(){
    return out("R");
}

pdsp::Patchable& pdsp::DimensionChorus::in_0(){
    return in("L");
}

pdsp::Patchable& pdsp::DimensionChorus::in_1(){
    return in("R");
}

pdsp::Patchable& pdsp::DimensionChorus::out_0(){
    return out("L");
}

pdsp::Patchable& pdsp::DimensionChorus::out_1(){
    return out("R");
}
