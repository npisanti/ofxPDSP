
#include "LowCut.h"

void pdsp::LowCut::patch(){
 
    addModuleInput("0", hp0a);
    addModuleInput("1", hp1a);
    addModuleOutput("0", out0);
    addModuleOutput("1", out1);
    
    addModuleInput("freq", freq);
    
    freq.set(10000.0f);
    
    freq >> hp0a.in_freq();
    freq >> hp0b.in_freq();
    freq >> hp1a.in_freq();
    freq >> hp1b.in_freq();
    
    hp0a.out_hpf() >> hp0b.out_hpf() >> out0;
    hp1a.out_hpf() >> hp1b.out_hpf() >> out1;
    
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
