
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

// datatable based polysynth

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
        void setup(PolySynth & m, int v);

        pdsp::PatchNode     voiceTrigger;
        
        pdsp::DataOscillator    oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               amp;


        pdsp::ADSR          envelope;    
    }; // end voice class -----------------------------


    // synth public API --------------------------------------

    void setup( int numVoice );
    
    pdsp::DataTable  datatable;

    pdsp::Patchable& ch( int index );

    vector<Voice>       voices;
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

    pdsp::Parameter     lfo_speed_ctrl;    
    pdsp::Parameter     lfo_wave_ctrl;

    pdsp::LFO           lfo;
    pdsp::Switch        lfo_switch;
    pdsp::ParameterAmp  lfo_filter_amt;    
    
    pdsp::LowCut			leakDC;  
    
    // chorus ------------------------
    pdsp::DimensionChorus   chorus;       
    ofParameterGroup    ui_chorus;
    pdsp::Parameter     chorus_speed_ctrl;
    pdsp::Parameter     chorus_depth_ctrl;
    
    std::vector<float> partials_vector;

};

