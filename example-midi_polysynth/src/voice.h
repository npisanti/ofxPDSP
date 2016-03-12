
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


struct SynthVoice{
    
    SynthVoice();
    SynthVoice(const SynthVoice& other);
    
    void patch();
    
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

    pdsp::CRSlew                slew;

    static ofxPDSPParameter     cutoffUI;
    static ofxPDSPParameter     resoUI;
    static ofxPDSPParameter     modAttackUI;
    static ofxPDSPParameter     modReleaseUI;
    
    static ofxPDSPParameter     pwUI;
    static ofxPDSPParameter     pwmSpeedUI;
    static ofxPDSPParameter     pwmUI;
 
};
