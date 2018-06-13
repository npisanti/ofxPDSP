
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
        void setup(PolySynth & m);

        pdsp::PatchNode     voiceTrigger;
        
        pdsp::DataOscillator    oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               voiceAmp;


        pdsp::ADSR          envelope;    
        pdsp::Amp           envToTable;
        pdsp::Amp           envToFilter;  
    }; // end voice class -----------------------------


    // synth public API --------------------------------------

    void setup( int numVoice );
    
    pdsp::DataTable  datatable;

    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();

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
    pdsp::Parameter     env_filter_ctrl;

    pdsp::Parameter     filter_lfo_mod_ctrl;    

    pdsp::Parameter     lfo_speed_ctrl;    
    pdsp::Parameter     lfo_wave_ctrl;

    pdsp::LFO           lfo;
    pdsp::Amp           lfoToFilter;
    pdsp::Switch        lfoSwitch;

    pdsp::LowCut			leakDC;  
    
    // chorus ------------------------
    pdsp::DimensionChorus   chorus;       
    ofParameterGroup    ui_chorus;
    pdsp::Parameter     chorus_speed_ctrl;
    pdsp::Parameter     chorus_depth_ctrl;
    
    std::vector<float> partials_vector;

};

