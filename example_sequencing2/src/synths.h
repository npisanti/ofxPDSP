
#pragma once

#include "ofxPDSP.h"

// you can create classes that can be patched with in("tag") and out("tag") 
// by declaring them sublcasses of pdsp::Patchable
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


class BassSynth : public pdsp::Patchable{
public:
    BassSynth() { patch(); }
    BassSynth( const BassSynth & other ) { patch(); }
    
    void patch (){
        //set inputs/outputs
        addModuleInput("trig", trigger_in);
        addModuleInput("pitch", osc.in_pitch());
        addModuleOutput("signal", amp );
        
        //patching
        osc.out_saw() * 2.5f >> drive >> filter >> amp;
        
        trigger_in >> ampEnv.set(0.0f, 50.0f, 1.0f, 200.0f) * 0.7f >> amp.in_mod();        
        trigger_in >> filterEnv.set(0.0f, 80.0f, 0.0f, 200.0f) * 60.0f >> filter.in_cutoff();
                                                                 50.0f >> filter.in_cutoff();
                                                                 0.2f  >> filter.in_reso();
    }
private:

    pdsp::Amp           amp;
    pdsp::VAOscillator  osc;
    pdsp::VAFilter     filter;
    pdsp::ADSR          ampEnv;
    pdsp::ADSR          filterEnv;
    pdsp::Saturator1    drive;
    pdsp::PatchNode     trigger_in;
};

