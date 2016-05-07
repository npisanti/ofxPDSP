
#include "KneeClipper.h"

void pdsp::KneeClipper::patch(){
 
    addModuleInput("0", input_gain0);
    addModuleInput("1", input_gain1);
    addModuleOutput("0", output_gain0);
    addModuleOutput("1", output_gain1);
    
    addModuleInput("threshold", threshold_ctrl);
    addModuleInput("knee", knee_ctrl);
    
    threshold_ctrl.set(0.0f);
    knee_ctrl.set(3.0f);
    
    knee_ctrl >> knee_gain_db >> threshold0.in_threshold();
                 knee_gain_db >> threshold1.in_threshold();
    
    threshold0.out_send() >> sat0 >> threshold0.in_return();
    threshold1.out_send() >> sat1 >> threshold1.in_return();
    
    threshold_ctrl >> input_gain_db >> input_gain0.in_mod();
                      input_gain_db >> input_gain1.in_mod();
                      
    threshold_ctrl * -1.0f >> output_gain_db >> output_gain0.in_mod();
                              output_gain_db >> output_gain1.in_mod();
                              
    input_gain0 >> threshold0 >> output_gain0;                          
    input_gain1 >> threshold1 >> output_gain1;                          
    
    
}

pdsp::Patchable& pdsp::KneeClipper::in_0(){
    return in("0");
}
    
pdsp::Patchable& pdsp::KneeClipper::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::KneeClipper::in_knee(){
    return in("freq");
}

pdsp::Patchable& pdsp::KneeClipper::in_threshold(){
    return in("threshold");
}


pdsp::Patchable& pdsp::KneeClipper::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::KneeClipper::out_1(){
    return out("1");
}
