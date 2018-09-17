
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
        void setup( PolySynth & m, int v );

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

    pdsp::Patchable & ch( int index );

    std::vector<Voice>       voices;
    ofParameterGroup    ui;

private: // --------------------------------------------------

    pdsp::ParameterGain gain;

    pdsp::Parameter     cutoff_ctrl;
    pdsp::Parameter     reso_ctrl;
    pdsp::Parameter     filter_mode_ctrl;

    pdsp::Parameter     env_attack_ctrl;
    pdsp::Parameter     env_decay_ctrl;
    pdsp::Parameter     env_sustain_ctrl;
    pdsp::Parameter     env_release_ctrl;
    
    pdsp::ParameterAmp  env_filter_amt;
    pdsp::ParameterAmp  env_table_amt;
    
    pdsp::Parameter     table_ctrl;

    pdsp::Parameter     table_lfo_mod_ctrl;    
    pdsp::Parameter     filter_lfo_mod_ctrl;    

    pdsp::Parameter     lfo_speed_ctrl;    
    pdsp::Parameter     lfo_wave_ctrl;
    
    pdsp::WaveTable  wavetable;

    pdsp::LFO           lfo;
    pdsp::Amp           lfo_to_table;
    pdsp::Amp           lfo_to_filter;
    pdsp::Switch        lfo_switch;

    // chorus ------------------------
    pdsp::DimensionChorus   chorus;         
    
    ofParameterGroup     ui_chorus;
    pdsp::Parameter      chorus_speed_ctrl;
    pdsp::Parameter      chorus_depth_ctrl;
    
    std::vector<float> partials_vector;

};

