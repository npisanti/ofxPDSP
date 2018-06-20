
#pragma once

// comment to deactivate legacy defines
#define OFXPDSP_LEGACY

#ifdef OFXPDSP_LEGACY

typedef pdsp::TriggerControl ofxPDSPTrigger;

typedef pdsp::Engine ofxPDSPEngine;

typedef pdsp::ComputerKeyboard ofxPDSPComputerKeyboard;

#ifndef __ANDROID__
typedef pdsp::midi::Input ofxPDSPMidiIn;
typedef pdsp::midi::Output ofxPDSPMidiOut;
typedef pdsp::midi::Keys ofxPDSPMidiKeys;
typedef pdsp::midi::Pads ofxPDSPMidiPads;
typedef pdsp::midi::Controls ofxPDSPMidiControls;
#endif

typedef pdsp::osc::Input ofxPDSPOscInput;
typedef pdsp::osc::Output ofxPDSPOscOutput;

typedef pdsp::Wrapper ofxPDSPWrapper;

typedef pdsp::Parameter ofxPDSPValue;
typedef pdsp::ParameterAmp ofxPDSPValueMultiplier;
typedef pdsp::ParameterGain ofxPDSPMonoFader;
typedef pdsp::ParameterGain ofxPDSPStereoFader;

typedef pdsp::SampleBufferPlotter ofxSampleBufferPlotter;
typedef pdsp::Scope ofxPDSPScope;

#ifndef TARGET_OS_IPHONE
#ifndef TARGET_IPHONE_SIMULATOR
#ifndef __ANDROID__
typedef pdsp::serial::Output ofxPDSPSerialOut;
#endif
#endif
#endif

namespace pdsp{
    typedef Delay DampedDelay;
    typedef ClockedPhasor ClockedPhazor;
    typedef LFOPhasor LFOPhazor;
    typedef PhasorShifter PhazorShifter;
    typedef PMPhasor PMPhazor;
    typedef WhiteNoise PRNoiseGen;
}

#endif
