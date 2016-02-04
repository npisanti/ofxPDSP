
#include "Compressor1.h"

void pdsp::Compressor1::patch(bool linkChannels){
    
    addUnitInput("0", input1);
    addUnitInput("1", input2);
    
    addUnitInput("attack", attack);
    addUnitInput("release", release);
    addUnitInput("threshold", threshold);
    addUnitInput("ratio", ratio);
    addUnitInput("knee", knee);

    addUnitOutput("0", dca1);
    addUnitOutput("1", dca2);

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
 
    input1 >> dca1;
    input2 >> dca2;
    
    
}

void pdsp::Compressor1::stereoLink(bool active){
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
        

pdsp::Patchable& pdsp::Compressor1::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::Compressor1::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::Compressor1::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::Compressor1::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::Compressor1::in_threshold(){
    return in("threshold");
}
pdsp::Patchable& pdsp::Compressor1::in_ratio(){
    return in("ratio");
}

pdsp::Patchable& pdsp::Compressor1::in_knee(){
    return in("knee");
}

pdsp::Patchable& pdsp::Compressor1::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::Compressor1::out_1(){
    return out("1");
}

float pdsp::Compressor1::meter_GR() const {
    return gr1.meter_reduction();
}
