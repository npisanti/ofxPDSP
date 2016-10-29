
#include "VAFilter.h"

const float pdsp::VAFilter::LowPass24 = 0.0f;
const float pdsp::VAFilter::LowPass12 = 1.0f;
const float pdsp::VAFilter::HighPass24 = 2.0f;
const float pdsp::VAFilter::HighPass12 = 3.0f;
const float pdsp::VAFilter::BandPass24 = 4.0f;
const float pdsp::VAFilter::BandPass12 = 5.0f;
            

void pdsp::VAFilter::patch(){
 
    addModuleInput("0", filterA );
    addModuleInput("1", filterB );
    addModuleOutput("0", switchA );
    addModuleOutput("1", switchB );
    
    addModuleInput("pitch", p2f);
    addModuleInput("reso", reso);
    addModuleInput("mode", mode);
    
    p2f.set(80.0f);
    
    p2f >> filterA.in_cutoff();
    p2f >> filterB.in_cutoff();
    reso >> filterA.in_reso();
    reso >> filterB.in_reso();
    
    switchA.resize(6);
    switchB.resize(6);

    mode.set(0.0f);

    mode >> switchA.in_select();
    filterA.out_lpf4() >> switchA.input(0);
    filterA.out_lpf2() >> switchA.input(1);
    filterA.out_hpf4() >> switchA.input(2);
    filterA.out_hpf2() >> switchA.input(3);
    filterA.out_bpf4() >> switchA.input(4);
    filterA.out_bpf2() >> switchA.input(5);
   
    mode >> switchB.in_select();    
    filterB.out_lpf4() >> switchB.input(0);
    filterB.out_lpf2() >> switchB.input(1);
    filterB.out_hpf4() >> switchB.input(2);
    filterB.out_hpf2() >> switchB.input(3);
    filterB.out_bpf4() >> switchB.input(4);
    filterB.out_bpf2() >> switchB.input(5);

}

pdsp::Patchable& pdsp::VAFilter::in_0(){
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::VAFilter::in_L(){
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_R(){
    return in("1");
}

pdsp::Patchable& pdsp::VAFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::VAFilter::in_cutoff(){
    return in("pitch");
}

pdsp::Patchable& pdsp::VAFilter::in_reso(){
    return in("reso");
}

pdsp::Patchable& pdsp::VAFilter::in_mode(){
    return in("mode");
}


pdsp::Patchable& pdsp::VAFilter::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::VAFilter::out_1(){
    return out("1");
}

pdsp::Patchable& pdsp::VAFilter::out_L(){
    return out("0");
}

pdsp::Patchable& pdsp::VAFilter::out_R(){
    return out("1");
}
