
#include "voice.h"

SynthUI::SynthUI(){
    
    controls.setName("synth parameters");
    controls.add(pw.set("pulse width", 0.5f, 0.0f, 1.0f) );
    controls.add(pwm.set("pwm amount", 0.3f, 0.0f, 0.5f) );
    controls.add(pwmSpeed.set("pwm speed (hz)", 0.5f, 0.005f, 1.5f));
    cutoff.enableSmoothing(200.0f);
    controls.add(cutoff.set("cutoff pitch", 82.6f, 10.0f, 120.0f));
    controls.add(reso.set("resonance", 0.0f, 0.0f, 1.0f) );
    controls.add(modAttack.set("mod env attack", 400.0f, 0.0f, 1200.0f) );
    controls.add(modRelease.set("mod env release", 900.0f, 0.0f, 1200.0f));    
    
}


// as we need an external object we will not patch on construction but later with this setup()
void SynthVoice::setup( SynthUI & ui ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", voicePitch);
    addModuleOutput("signal", voiceAmp);

    // SIGNAL PATH
    oscillator.out("pulse") * 4.0f >> saturator >> filter >> voiceAmp;
    
    // MODULATIONS AND CONTROL
                 ui.pwm >> lfoAmt.in_mod();
    lfo.out_triangle() >> lfoAmt >> oscillator.in_pw();
                           ui.pw >> oscillator.in_pw();
                      voicePitch >> oscillator.in_pitch();
                                                             
                                                              ui.cutoff >> p2f;
     voiceTrigger >> (modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) * 48.0f) >> p2f >> filter.in_cutoff(); 
                                                                       ui.reso >> filter.in_reso();
       ui.modAttack >> modEnv.in_attack();
      ui.modRelease >> modEnv.in_release();
     
    (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
    
                     ui.pwmSpeed >> lfo.in_freq();  
                     
}


float SynthVoice::meter_mod_env() const{
    return modEnv.meter_output();
}

float SynthVoice::meter_pitch() const{
    return oscillator.meter_pitch();
}
