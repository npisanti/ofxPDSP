#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

class Zap : public pdsp::Patchable{ 
public:
    Zap(){ patch(); }
    Zap(const Zap & other){ patch(); }

    void patch();

    void enableDB( float minValue=-18.0f );

    float meter_env() const;

    ofxPDSPValue        pitchControl;

private:
    pdsp::PatchNode     triggers;
    pdsp::FMOperator    osc;
    pdsp::AHR           aEnv;
    pdsp::AHR           pEnv;
    pdsp::Amp           amp;
    pdsp::Amp           pModAmt;
    pdsp::Amp           fbModAmt;
    
};


class Zaps {

public:
    void setup( int numVoices );
    ofParameterGroup ui;
    
    vector<Zap> voices;
    ofxPDSPStereoFader fader;
private:
    ofxPDSPValue veloToEnv;
    ofxPDSPValue envAmtControl;
    ofxPDSPValue envDecayControl;

};

