
#include "LinearCrossfader.h"

pdsp::LinearCrossfader::LinearCrossfader() { patch(); }
pdsp::LinearCrossfader::LinearCrossfader(const LinearCrossfader& other){ patch(); }
pdsp::LinearCrossfader& pdsp::LinearCrossfader::operator=(const LinearCrossfader& other){ return *this; }

void pdsp::LinearCrossfader::patch(){
                 
    addModuleInput( "A",  in1amp );
    addModuleInput( "B",  in2amp );    
    addModuleInput("fade", control);
    addModuleOutput("signal", output);
    
    control.enableBoundaries(0.0f, 1.0f);
    
    control >> oneMinusX >> in1amp.in_mod();
                 control >> in2amp.in_mod();    
                 
    in1amp >> output;
    in2amp >> output;
}

float pdsp::LinearCrossfader::meter_fade(){
    return in2amp.meter_mod();
}

pdsp::Patchable& pdsp::LinearCrossfader::in_A(){
    return in("A");
}

pdsp::Patchable& pdsp::LinearCrossfader::in_B(){
    return in("B");
}

pdsp::Patchable& pdsp::LinearCrossfader::in_fade(){
    return in("fade");    
} 

pdsp::Patchable& pdsp::LinearCrossfader::out_signal(){
    return out("signal");    
} 

// ----------------- backward compatibility -------------------------
pdsp::Patchable& pdsp::LinearCrossfader::in_0(){
    return in("A");
}

pdsp::Patchable& pdsp::LinearCrossfader::in_1(){
    return in("B");
}


