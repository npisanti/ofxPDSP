
#pragma once

// comment to deactivate legacy defines
#define OFXPDSP_LEGACY

#ifdef OFXPDSP_LEGACY

typedef pdsp::TriggerControl ofxPDSPTrigger;

typedef pdsp::Engine ofxPDSPEngine;

typedef pdsp::ComputerKeyboard ofxPDSPComputerKeyboard;

typedef pdsp::midi::Input ofxPDSPMidiIn;
typedef pdsp::midi::Output ofxPDSPMidiOut;
typedef pdsp::midi::Keys ofxPDSPMidiKeys;
typedef pdsp::midi::Pads ofxPDSPMidiPads;
typedef pdsp::midi::Controls ofxPDSPMidiControls;

typedef pdsp::osc::Input ofxPDSPOscInput;
typedef pdsp::osc::Output ofxPDSPOscOutput;

typedef pdsp::Wrapper ofxPDSPWrapper;

typedef pdsp::Parameter ofxPDSPValue;
typedef pdsp::ParameterAmp ofxPDSPValueMultiplier;
typedef pdsp::ParameterGain ofxPDSPMonoFader;
typedef pdsp::ParameterGain ofxPDSPStereoFader;

typedef pdsp::SampleBufferPlotter ofxSampleBufferPlotter;
typedef pdsp::Scope ofxPDSPScope;

typedef pdsp::serial::Output ofxPDSPSerialOut;

namespace pdsp{
    typedef Delay DampedDelay;
    typedef ClockedPhasor ClockedPhazor;
    typedef LFOPhasor LFOPhazor;
    typedef PhasorShifter PhazorShifter;
    typedef PMPhasor PMPhazor;
    typedef WhiteNoise PRNoiseGen;
}

#endif
