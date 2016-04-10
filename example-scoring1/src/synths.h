
#pragma once

#include "ofxPDSP.h"

class SineBleep : public pdsp::Patchable{
public:
    SineBleep() {
        //add inputs / outputs
        addModuleInput("trig", env.in_trig()); 
        addModuleInput("pitch", osc.in_pitch());
        addModuleOutput("signal", amp ); 
        
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
    BassSynth() {

        //add inputs/outputs
        addModuleInput("trig", trigger_in);
        addModuleInput("pitch", osc.in_pitch());
        addModuleOutput("signal", amp );

        // patching
        osc.out_saw() * 2.5f >> drive >> filter.out_lpf4() >> amp;

        trigger_in >> ampEnv.set(0.0f, 100.0f, 200.0f) * 0.7f >> amp.in_mod();        
        trigger_in >> filterEnv.set(0.0f, 0.0f, 200.0f) * 48.0f >> p2f; 
                                                         50.0f >> p2f >> filter.in_cutoff();
                                                                0.5f  >> filter.in_reso();
    }
    
private:
    pdsp::PitchToFreq   p2f; 
    pdsp::Amp           amp;
    pdsp::VAOscillator  osc;
    pdsp::MultiLadder4  filter;
    pdsp::AHR           ampEnv;
    pdsp::AHR           filterEnv;
    pdsp::Saturator1    drive;
    pdsp::PatchNode     trigger_in;
    
    
    pdsp::PRNoiseGen    noise;
    pdsp::AHR env;
    
};

