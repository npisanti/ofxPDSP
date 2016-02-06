
#include "voice.h"

// we use static instances for the parameters so we can patch a single parameter to many voices
ofxPDSPParameter SynthVoice::cutoffUI     = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::resoUI       = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::pwUI         = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::pwmUI        = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::pwmSpeedUI   = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::modAttackUI  = ofxPDSPParameter();
ofxPDSPParameter SynthVoice::modReleaseUI = ofxPDSPParameter();
ofParameterGroup SynthVoice::synthUI = ofParameterGroup();
bool SynthVoice::initUIflag = true;

SynthVoice::SynthVoice() { 
    patch(); 
    if(initUIflag){
        initUI();
        initUIflag = false; // we add the parameter to the ofParameterGroup just once
    }
}

SynthVoice::SynthVoice(const SynthVoice& other){ patch(); }

void SynthVoice::patch(){
    
    // SIGNAL PATH
    oscillator.out("pulse") * 4.0f >> saturator >> filter >> voiceAmp;
    
    // MODULATIONS AND CONTROL
                    pwmUI.set("pwm amount", 0.3f, 0.0f, 0.5f) >> lfoAmt.in_mod();
                                           lfo.out_triangle() >> lfoAmt >> oscillator.in_pw();
                            pwUI.set("pulse width", 0.5f, 0.0f, 1.0f)   >> oscillator.in_pw();
                                                             voicePitch >> oscillator.in_pitch();
                                                             
    // the ofxPDSPParameters are static instance so we are patching them to all the voices here
 cutoffUI.set("cutoff pitch", 82.6f, 10.0f, 120.0f) >> slew.set(200.0f) >> p2f;
     voiceTrigger >> (modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) * 48.0f) >> p2f >> filter.in_cutoff(); 
                                    resoUI.set("resonance", 0.0f, 0.0f, 1.0f)  >> filter.in_reso();
     
     modAttackUI.set("mod env attack", 400.0f, 0.0f, 1200.0f) >> modEnv.in_attack();
     modReleaseUI.set("mod env release", 900.0f, 0.0f, 1200.0f) >> modEnv.in_release();
     
    (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
    
                     pwmSpeedUI.set("pwm speed (hz)", 0.5f, 0.005f, 1.5f) >> lfo.in_freq();  
}

void SynthVoice::initUI(){
    synthUI.setName("synth parameters");
    synthUI.add(pwUI.parameter);
    synthUI.add(pwmUI.parameter);    
    synthUI.add(pwmSpeedUI.parameter);   
    synthUI.add(cutoffUI.parameter);
    synthUI.add(resoUI.parameter);
    synthUI.add(modAttackUI.parameter);
    synthUI.add(modReleaseUI.parameter);
}
