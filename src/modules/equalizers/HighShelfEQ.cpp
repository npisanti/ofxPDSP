
#include "HighShelfEQ.h"

void pdsp::HighShelfEQ::patch(){
 
    addModuleInput("0", eq0);
    addModuleInput("1", eq1);
    addModuleOutput("0", eq0);
    addModuleOutput("1", eq1);
    
    addModuleInput("freq", freq);
    addModuleInput("Q", Q);
    addModuleInput("gain", gain);
    
    freq.set(10000.0f);
    Q.set(0.707f);
    gain.set(0.0f);
    
    freq >> eq0.in_freq();
    freq >> eq1.in_freq();
    Q >> eq0.in_Q();
    Q >> eq1.in_Q();
    gain >> eq0.in_gain();
    gain >> eq1.in_gain();
    
}

pdsp::Patchable& pdsp::HighShelfEQ::in_0(){
    return in("0");
}
    
pdsp::Patchable& pdsp::HighShelfEQ::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_Q(){
    return in("Q");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_gain(){
    return in("gain");
}

pdsp::Patchable& pdsp::HighShelfEQ::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::HighShelfEQ::out_1(){
    return out("1");
}
