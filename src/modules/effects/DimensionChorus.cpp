
#include "DimensionChorus.h"

pdsp::DimensionChorus::DimensionChorus() { patch(); }
pdsp::DimensionChorus::DimensionChorus(const DimensionChorus& other){ patch(); }
pdsp::DimensionChorus& pdsp::DimensionChorus::operator=(const DimensionChorus& other){ return *this; }

//pdsp::DimensionChorus::DimensionChorus(DimensionChorus&& other){ patch(); }
//pdsp::DimensionChorus::DimensionChorus& operator=( DimensionChorus&& other){ return *this; }

void pdsp::DimensionChorus::patch(){
    
    addUnitInput( "0",  input1 );
    addUnitInput( "1",  input2 );    
    addUnitOutput("0", output1);
    addUnitOutput("1", output2);
    
    addUnitInput("speed", speed);
    addUnitInput("depth", depth);
    addUnitInput("delay", delay);
    
    speed.set(0.25);
    speed.enableBoundaries(0.15f, 0.6f);
    depth.set(10.0f);
    depth.enableBoundaries(0.5f, 30.0f);
    delay.set(80.0f);
    delay.enableBoundaries(40.0f, 100.0f);


                       input1 >>           output1;
                       input1 >> delay1 >> output1;
    input2 * -1.0f >> filter2.out_hpf() >> output1;
    
                       input2 >>           output2;
                       input2 >> delay2 >> output2;
    input1 * -1.0f >> filter1.out_hpf() >> output2;
                    
        
    speed >> phazor.in_freq() >> LFO >> mod1  >> delay1.in_time();
                                        delay >> delay1.in_time();
                                 LFO >> mod2  >> delay2.in_time();
                                        delay >> delay2.in_time();
                                        
    depth         >> mod1.in_mod();
    depth * -1.0f >> mod2.in_mod();
                         
    filter1.set(500.0f); // set default cutoff
    filter2.set(500.0f); // set default cutoff                              
}

pdsp::Patchable& pdsp::DimensionChorus::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::DimensionChorus::in_1(){
    return in("1");
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

pdsp::Patchable& pdsp::DimensionChorus::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::DimensionChorus::out_1(){
    return out("1");
}

float pdsp::DimensionChorus::meter_lfo(){
    return LFO.meter_output();
}

