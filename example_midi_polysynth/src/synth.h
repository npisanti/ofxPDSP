
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

class PolySynth : public pdsp::Patchable {

public:
    class Voice : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        void setup(PolySynth & ui, int v);
        
        float meter_mod_env() const;
        float meter_pitch() const;

    private:
        pdsp::PatchNode     voiceTrigger;
        pdsp::PatchNode     voicePitch;
        
        pdsp::VAOscillator  oscillator;

        pdsp::Saturator1    saturator;
        pdsp::VAFilter      filter;
        pdsp::Amp           voiceAmp;
        
        pdsp::ADSR          ampEnv;
        pdsp::ADSR          modEnv;
        
        pdsp::LFO           lfo;
     
    }; // end Voice class -------------------------------------------------------

    // public API --------------------------------------------------------------
    void setup( int numVoices );

    ofParameterGroup    ui;
    std::vector<Voice>  voices;
    
private: // --------------------------------------------------------------------
    pdsp::Parameter     cutoff;
    pdsp::Parameter     reso;
    pdsp::Parameter     modAttack;
    pdsp::Parameter     modRelease;
    pdsp::ParameterAmp  modAmt;
    
    pdsp::Parameter     pw;
    pdsp::Parameter     pwmSpeed;
    pdsp::ParameterAmp  pwmAmt;    

};



