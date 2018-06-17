
#pragma once

#include "ofxPDSP.h"

class SineBleep : public pdsp::Patchable{
public:
    SineBleep() { patch(); } 
    SineBleep(const SineBleep & other) { patch(); } 

    void patch() {
        //add inputs / outputs with these methods 
        addModuleInput("trig", env.in_trig()); // arguments are tag and the Unit in/out to link to that tag
        addModuleInput("pitch", osc.in_pitch());
        addModuleOutput("signal", amp ); // if in/out is not selected default in/out is used
        
        //patching
        env.set(0.0f, 100.0f, 350.0f) * 0.25f >> amp.in_mod();
        env * 0.10f           >> osc.in_fb() >> amp;
    }
private:
    pdsp::Amp amp;
    pdsp::FMOperator osc;
    pdsp::AHR env;
    
};


class KickSynth : public pdsp::Patchable{
public:
    KickSynth() { patch(); }
    KickSynth( const KickSynth & other ) { patch(); }
    
    void patch (){
        //set inputs/outputs
        addModuleInput("trig", trigger_in);
        addModuleOutput("signal", amp );
        
        //patching
                                                  osc >> amp;
        trigger_in >> ampEnv.set(0.0f, 50.0f, 100.0f) >> amp.in_mod();        
        trigger_in >> modEnv.set(0.0f, 0.0f, 50.0f) * 48.0f >> osc.in_pitch();
                                                      48.0f >> osc.in_pitch();
        ampEnv.enableDBTriggering( -24.0f );
    }
private:
    pdsp::Amp           amp;
    pdsp::FMOperator    osc;
    pdsp::AHR           ampEnv;
    pdsp::AHR           modEnv;
    pdsp::PatchNode     trigger_in;
};

