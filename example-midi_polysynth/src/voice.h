
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


// this class contains the ui controls

struct SynthUI {

    SynthUI();

    ofParameterGroup controls;

    ofxPDSPValue     cutoff;
    ofxPDSPValue     reso;
    ofxPDSPValue     modAttack;
    ofxPDSPValue     modRelease;
    
    ofxPDSPValue     pw;
    ofxPDSPValue     pwmSpeed;
    ofxPDSPValue     pwm;    

};

// this class rapresent each synth voice

struct SynthVoice : public pdsp::Patchable {
    
    SynthVoice(){}
    SynthVoice(const SynthVoice& other){}
    
    void setup(SynthUI & ui);
    
    float meter_mod_env() const;
    float meter_pitch() const;

private:

    pdsp::PatchNode     voiceTrigger;
    pdsp::PatchNode     voicePitch;
    pdsp::PitchToFreq   p2f;
    
    pdsp::VAOscillator  oscillator;

    pdsp::Saturator1    saturator;
    pdsp::MultiLadder4  filter;
    pdsp::Amp           voiceAmp;
    
    pdsp::ADSR          ampEnv;
    pdsp::ADSR          modEnv;
    
    pdsp::LFO           lfo;
    pdsp::Amp           lfoAmt;
 
};
