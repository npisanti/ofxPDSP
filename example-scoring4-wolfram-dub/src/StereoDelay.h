
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


class StereoDelay : public pdsp::Patchable {

public:
    StereoDelay() { patch(); };
    StereoDelay(const StereoDelay & other) { patch(); };
    
    ofParameterGroup ui;

    ofxPDSPValueMultiplier lDelayTimeControl;
    ofxPDSPValueMultiplier rDelayTimeControl;
    
    ofxPDSPValue lFeedbackControl;
    ofxPDSPValue rFeedbackControl;
    
private:
    void patch();


    pdsp::DampedDelay ldelay;
    pdsp::DampedDelay rdelay;
    
    pdsp::OneBarTimeMs time;

    ofxPDSPValue dampingControl;
    
    ofxPDSPStereoFader input;
    ofxPDSPStereoFader output;
    
};

