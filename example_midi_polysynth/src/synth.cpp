
#include "synth.h"

void PolySynth::setup(int numVoices ){
    
    voices.resize( numVoices );
    
    for( size_t i=0; i<voices.size(); ++i ){
        voices[i].setup( *this, i );
    }   
    
    ui.setName("synth parameters");
    ui.add(pw.set("pulse width", 0.5f, 0.0f, 1.0f) );
    ui.add(pwmAmt.set("pwm amount", 0.3f, 0.0f, 0.5f) );
    ui.add(pwmSpeed.set("pwm speed (hz)", 0.5f, 0.005f, 1.5f));
    cutoff.enableSmoothing(200.0f);
    ui.add(cutoff.set("cutoff pitch", 82, 20, 136));
    ui.add(reso.set("resonance", 0.0f, 0.0f, 1.0f) );
    ui.add(modAttack.set("mod env attack", 400, 0, 1200) );
    ui.add(modRelease.set("mod env release", 900, 0, 1200));    
    ui.add(modAmt.set("mod env amount", 48, 0, 84));    
    
}


void PolySynth::Voice::setup( PolySynth & ui, int v ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", voicePitch);
    addModuleInput("cutoff", filter.in_cutoff() );
    addModuleOutput("signal", voiceAmp);

    // SIGNAL PATH
    oscillator.out("pulse") * 4.0f >> saturator >> filter >> voiceAmp;
    
    // MODULATIONS AND CONTROL
    lfo.out_triangle() >> ui.pwmAmt.ch(v) >> oscillator.in_pw();
                                    ui.pw >> oscillator.in_pw();
                               voicePitch >> oscillator.in_pitch();
                                                             
                                                                       ui.cutoff >> filter.in_cutoff(); 
     voiceTrigger >> modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) >> ui.modAmt.ch(v) >> filter.in_cutoff(); 
                                                                         ui.reso >> filter.in_reso();
    
    (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();

       ui.modAttack >> modEnv.in_attack();
      ui.modRelease >> modEnv.in_release();
        ui.pwmSpeed >> lfo.in_freq();    
}


float PolySynth::Voice::meter_mod_env() const{
    return modEnv.meter_output();
}

float PolySynth::Voice::meter_pitch() const{
    return oscillator.meter_pitch();
}
