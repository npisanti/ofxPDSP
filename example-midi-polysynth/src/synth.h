
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
        
        void setup(PolySynth & ui);
        
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
     
    }; // end Voice class -------------------------------------------------------

    // public API --------------------------------------------------------------
    void setup( int numVoices );

    ofParameterGroup    ui;
    vector<Voice>       voices;
    
private: // --------------------------------------------------------------------
    ofxPDSPValue     cutoff;
    ofxPDSPValue     reso;
    ofxPDSPValue     modAttack;
    ofxPDSPValue     modRelease;
    
    ofxPDSPValue     pw;
    ofxPDSPValue     pwmSpeed;
    ofxPDSPValue     pwm;    

};



