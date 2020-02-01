
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "flags.h"
#include "Automaton1D.h"

#define NUMSYNTHS 4

class WolframSeq : public pdsp::Sequence {

public:
    WolframSeq();

    // if you want to make something less hacky you should use proper std::atomic values instead of ofParameter(s)
    ofParameterGroup parameters;
    ofParameter<int>     threshold;
    ofParameter<int>     activeOuts;
    ofParameter<int>     steps;
    ofParameter<int>     rule;
    ofParameter<float>   density;
    ofParameter<bool>    regenerate;

    void draw( int ca_side, int bars_h, ofColor fg, ofColor bg );
    void setRule(int rule){ ca.setRule( rule); }
    int currentStep() const;
    float getStep( int step, int out ) const;

    pdsp::SequencerGateOutput& out_trig( int index );
    
    vector<float>   stepbars;    
    
private:
    pdsp::Function seq;
    Automaton1D     ca;
    int ruleMem;
    
    std::atomic<int> meter_step;

};
