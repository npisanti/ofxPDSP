
#include "Compressor2.h"

void pdsp::Compressor2::patch(bool linkChannels){
    
    addModuleInput("0", input1);
    addModuleInput("1", input2);
    
    addModuleInput("attack", attack);
    addModuleInput("release", release);
    addModuleInput("threshold", threshold);
    addModuleInput("ratio", ratio);
    addModuleInput("knee", knee);

    addModuleOutput("0", dca1);
    addModuleOutput("1", dca2);

    attack.set(10.0f);
    release.set(50.0f),
    ratio.set(4.0f);
    threshold.set(-20.0f);
    knee.set(0.0f);

    attack >> envelope1.in_attack();
    attack >> envelope2.in_attack();
    release >> envelope1.in_release();
    release >> envelope2.in_release();
    threshold >> gr1.in_threshold();
    threshold >> gr2.in_threshold();
    ratio >> gr1.in_ratio();
    ratio >> gr2.in_ratio();
    knee >> gr1.in_knee();
    knee >> gr2.in_knee();
    
    input1 >> detector1;
    input2 >> detector2;

    stereoLink( linkChannels );
 
 
    setRMSWindow(50.0f, false);    
    
}

void pdsp::Compressor2::stereoLink(bool active){
    if(active){
        envelope1.disconnectIn();
        dca2.in_mod().disconnectIn();
        
        detector1 >> stereoLinker.in_0();
        detector2 >> stereoLinker.in_1();
                      stereoLinker >> envelope1 >> linToDB1 >> gr1 >> dbToLin1 >> dca1.in_mod();
                                                                      dbToLin1 >> dca2.in_mod();
    }else{
        envelope1.disconnectIn();        
        dca2.in_mod().disconnectIn();
        
        detector1 >> envelope1 >> linToDB1 >> gr1 >> dbToLin1 >> dca1.in_mod();
        detector2 >> envelope2 >> linToDB2 >> gr2 >> dbToLin2 >> dca2.in_mod();
    }
}

void pdsp::Compressor2::setRMSWindow(float window_ms, bool lookahead){
    detector1.set(window_ms);
    detector2.set(window_ms);
    
    if(lookahead){
        delay1.setSamples(detector1.getWindowSamples());
        delay2.setSamples(detector2.getWindowSamples());
        dca1.in_signal().disconnectIn();
        dca2.in_signal().disconnectIn();
        input1 >> delay1 >> dca1;
        input2 >> delay2 >> dca2;        
    }else{
        dca1.in_signal().disconnectIn();
        dca2.in_signal().disconnectIn();
        input1 >> dca1;
        input2 >> dca2;     
    }
    
}

pdsp::Patchable& pdsp::Compressor2::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::Compressor2::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::Compressor2::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::Compressor2::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::Compressor2::in_threshold(){
    return in("threshold");
}
pdsp::Patchable& pdsp::Compressor2::in_ratio(){
    return in("ratio");
}

pdsp::Patchable& pdsp::Compressor2::in_knee(){
    return in("knee");
}

pdsp::Patchable& pdsp::Compressor2::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::Compressor2::out_1(){
    return out("1");
}

float pdsp::Compressor2::meter_GR() const {
    return gr1.meter_reduction();
}
