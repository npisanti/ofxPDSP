
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

// wavetable based polysynth

class PolySynth {

public:
    // class to rapresent each synth voice ------------
    class Voice : public pdsp::Patchable {
        friend class PolySynth;
    
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        float meter_mod_env() const;
        float meter_pitch() const;

    private:
        void setup(PolySynth & m);

        pdsp::PatchNode     voiceTrigger;
        
        pdsp::TableOscillator   oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               voiceAmp;


        pdsp::ADSR          envelope;    
        pdsp::Amp           envToTable;
        pdsp::Amp           envToFilter;  
    }; // end voice class -----------------------------


    // synth public API --------------------------------------

    void setup( int numVoice );

    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();

    vector<Voice>       voices;
    ofParameterGroup    ui;

private: // --------------------------------------------------

    ofxPDSPStereoFader gain;

    ofxPDSPValue     cutoff_ctrl;
    ofxPDSPValue     reso_ctrl;
    ofxPDSPValue     filter_mode_ctrl;

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

    // chorus ------------------------
    pdsp::DimensionChorus   chorus;         
    
    ofParameterGroup     ui_chorus;
    ofxPDSPValue     chorus_speed_ctrl;
    ofxPDSPValue     chorus_depth_ctrl;
    
    vector<float> partials_vector;

};

