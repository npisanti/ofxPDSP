
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


class StereoDelay : public pdsp::Patchable {

public:
    StereoDelay() { patch(); };
    StereoDelay(const StereoDelay & other) { patch(); };
    
    ofParameterGroup ui;

    pdsp::ParameterAmp lDelayTimeControl;
    pdsp::ParameterAmp rDelayTimeControl;
    
    pdsp::Parameter lFeedbackControl;
    pdsp::Parameter rFeedbackControl;
    
private:
    void patch();


    pdsp::Delay ldelay;
    pdsp::Delay rdelay;
    
    pdsp::OneBarTimeMs time;

    pdsp::Parameter dampingControl;
    
    pdsp::ParameterGain input;
    pdsp::ParameterGain output;
    
};

