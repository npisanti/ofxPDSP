
#include "Compressor.h"

void pdsp::Compressor::patch(bool linkChannels){
    
    addModuleInput("0", input1);
    addModuleInput("1", input2);
    
    addModuleInput("attack", attack);
    addModuleInput("release", release);
    addModuleInput("threshold", threshold);
    addModuleInput("ratio", ratio);
    addModuleInput("knee", knee);

    addModuleOutput("0", dca1);
    addModuleOutput("1", dca2);
    
    // 300 ms is already default
    //delay1.setMaxTime(300.0f);
    //delay2.setMaxTime(300.0f);

    attack.set(10.0f);
    release.set(50.0f),
    ratio.set(4.0f);
    threshold.set(-20.0f);
    knee.set(0.0f);

    attack >> delay1.in_time();
    attack >> delay2.in_time();

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

    20.0f >> leakDC1.in_freq();
    20.0f >> leakDC2.in_freq();    
    input1 >> leakDC1.out_hpf() >> rms1;
    input2 >> leakDC2.out_hpf() >> rms2;
              leakDC1.out_hpf() >> peak1;
              leakDC2.out_hpf() >> peak2;

    lh = false;
    rms = false;
    linked = linkChannels;
    repatch();
 
    input1 >> dca1;
    input2 >> dca2;
}

void pdsp::Compressor::analog(){
    envelope1.analog();
    envelope2.analog();
    lh = false;
    repatch();
}

void pdsp::Compressor::digital( bool lookahead ){
    envelope1.digital();
    envelope2.digital();
    lh = lookahead;
    repatch();
}

void pdsp::Compressor::stereoLink(bool active){
    linked = active;
    repatch();
}

void pdsp::Compressor::repatch(){

    dca1.in_signal().disconnectIn();
    dca2.in_signal().disconnectIn();
    envelope1.disconnectIn();
    envelope2.disconnectIn();
    dca2.in_mod().disconnectIn();
                
    if( lh ){
        input1 >> delay1 >> dca1;
        input2 >> delay2 >> dca2;        
    }else{
        input1 >> dca1;
        input2 >> dca2;     
    }    

    if( linked ){
        
        if(rms){
            rms1 >> stereoLinker.in_0();
            rms2 >> stereoLinker.in_1();            
        }else{
            peak1 >> stereoLinker.in_0();
            peak2 >> stereoLinker.in_1();            
        }
        
        stereoLinker >> envelope1 >> linToDB1 >> gr1 >> dbToLin1 >> dca1.in_mod();
                                                        dbToLin1 >> dca2.in_mod();
    }else{
        if(rms){
            rms1 >> envelope1;
            rms2 >> envelope2;            
        }else{
            peak1 >> envelope1;
            peak2 >> envelope2;      
        }
        envelope1 >> linToDB1 >> gr1 >> dbToLin1 >> dca1.in_mod();
        envelope2 >> linToDB2 >> gr2 >> dbToLin2 >> dca2.in_mod();
    }
}

void pdsp::Compressor::RMS( float window_ms ){
    rms1.set(window_ms);
    rms2.set(window_ms);
    rms = true;
    repatch();   
}

void pdsp::Compressor::peak(){
    rms = false;
    repatch(); 
}

pdsp::Patchable& pdsp::Compressor::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::Compressor::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::Compressor::in_attack(){
    return in("attack");
}

pdsp::Patchable& pdsp::Compressor::in_release(){
    return in("release");
}

pdsp::Patchable& pdsp::Compressor::in_threshold(){
    return in("threshold");
}
pdsp::Patchable& pdsp::Compressor::in_ratio(){
    return in("ratio");
}

pdsp::Patchable& pdsp::Compressor::in_knee(){
    return in("knee");
}

pdsp::Patchable& pdsp::Compressor::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::Compressor::out_1(){
    return out("1");
}

float pdsp::Compressor::meter_GR() const {
    return gr1.meter_reduction();
}
