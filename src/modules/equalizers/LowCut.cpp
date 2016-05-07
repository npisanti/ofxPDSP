
#include "LowCut.h"

void pdsp::LowCut::patch(){
 
    addModuleInput("0", hp0a);
    addModuleInput("1", hp1a);
    addModuleOutput("0", hp0b.out_hpf());
    addModuleOutput("1", hp1b.out_hpf());
    
    addModuleInput("freq", freq);
    
    freq.set(10000.0f);
    
    freq >> hp0a.in_cutoff();
    freq >> hp0b.in_cutoff();
    freq >> hp1a.in_cutoff();
    freq >> hp1b.in_cutoff();
    
    hp0a.out_hpf() >> hp0b;
    hp1a.out_hpf() >> hp1b;
    
}

pdsp::Patchable& pdsp::LowCut::in_0(){
    return in("0");
}
    
pdsp::Patchable& pdsp::LowCut::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::LowCut::in_freq(){
    return in("freq");
}


pdsp::Patchable& pdsp::LowCut::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::LowCut::out_1(){
    return out("1");
}
