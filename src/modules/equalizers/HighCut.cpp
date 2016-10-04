
#include "HighCut.h"

void pdsp::HighCut::patch(){
 
    addModuleInput("0", lp0a );
    addModuleInput("1", lp1a );
    addModuleOutput("0", lp0b );
    addModuleOutput("1", lp1b );
    
    addModuleInput("freq", freq);
    
    freq.set(10000.0f);
    
    freq >> lp0a.in_cutoff();
    freq >> lp0b.in_cutoff();
    freq >> lp1a.in_cutoff();
    freq >> lp1b.in_cutoff();
    
    lp0a >> lp0b;
    lp1a >> lp1b;
    
}

pdsp::Patchable& pdsp::HighCut::in_0(){
    return in("0");
}
    
pdsp::Patchable& pdsp::HighCut::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::HighCut::in_freq(){
    return in("freq");
}


pdsp::Patchable& pdsp::HighCut::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::HighCut::out_1(){
    return out("1");
}
