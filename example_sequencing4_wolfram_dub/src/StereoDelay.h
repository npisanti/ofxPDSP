
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


class StereoDelay : public pdsp::Patchable {

public:
    StereoDelay() { patch(); };
    StereoDelay(const StereoDelay & other) { patch(); };
    
    ofParameterGroup parameters;

    pdsp::ParameterAmp lDelayTimeControl;
    pdsp::ParameterAmp rDelayTimeControl;
    
    pdsp::Parameter lFeedbackControl;
    pdsp::Parameter rFeedbackControl;

    pdsp::Patchable & ch( int index );

private:
    struct Submodule : public pdsp::Patchable {
        Submodule();
        pdsp::PatchNode input;
        pdsp::PatchNode output;
    };
    
    std::vector<Submodule> channels;

    pdsp::ParameterGain inputFader;
    pdsp::ParameterGain outputFader;

    void patch();

    pdsp::Delay delay0;
    pdsp::Delay delay1;
    
    pdsp::OneBarTimeMs time;

    pdsp::Parameter dampingControl;
    
};

