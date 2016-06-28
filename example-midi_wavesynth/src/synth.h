
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

// this class contains the ui controls

struct SynthGlobal {

    SynthGlobal();

    void addToGUI( ofxPanel & gui );

    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();

    ofParameterGroup ui_osc;
    ofParameterGroup ui_filter;
    ofParameterGroup ui_lfo;
    ofParameterGroup ui_mod_env;
    ofParameterGroup ui_amp_env;

    ofxPDSPValue     cutoff_ctrl;
    ofxPDSPValue     reso_ctrl;

    ofxPDSPValue     env_attack_ctrl;
    ofxPDSPValue     env_decay_ctrl;
    ofxPDSPValue     env_sustain_ctrl;
    ofxPDSPValue     env_release_ctrl;
    ofxPDSPValue     env_filter_ctrl;
    ofxPDSPValue     env_table_ctrl;
    
    ofxPDSPValue     table_ctrl;

    ofxPDSPValue     table_lfo_mod_ctrl;    
    ofxPDSPValue     filter_lfo_mod_ctrl;    

    ofxPDSPValue     lfo_speed_ctrl;    
    ofxPDSPValue     lfo_wave_ctrl;
    
    pdsp::WaveTable  wavetable;

    pdsp::LFO           lfo;
    pdsp::Amp           lfoToTable;
    pdsp::Amp           lfoToFilter;
    pdsp::Switch        lfoSwitch;

    // chorus
    pdsp::DimensionChorus   chorus;         
    
    ofParameterGroup     ui_chorus;

    ofxPDSPValue     chorus_speed_ctrl;
    ofxPDSPValue     chorus_depth_ctrl;

};

// this class rapresent each synth voice

struct SynthVoice : public pdsp::Patchable {
    
    SynthVoice(){}
    SynthVoice(const SynthVoice& other){}
    
    void setup(SynthGlobal & m);
    
    float meter_mod_env() const;
    float meter_pitch() const;

private:

    pdsp::TableOscillator oscillator;

    pdsp::PatchNode     voiceTrigger;
    pdsp::PatchNode     voicePitch;
    pdsp::PitchToFreq   p2f;
    
    
    pdsp::MultiLadder4  filter;
    pdsp::Amp           voiceAmp;
    
    pdsp::ADSR          envelope;    
    pdsp::Amp        envToTable;
    pdsp::Amp        envToFilter;  
};
