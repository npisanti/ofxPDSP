
#pragma once

#include "ofxPDSP.h"


class AudioAnalyzerBand {

public:
    AudioAnalyzerBand(){ }
    AudioAnalyzerBand( const AudioAnalyzerBand & other ){ }
    
    void setup( ofxPDSPEngine & engine, string name, int input = 0 );
    
    ofParameterGroup        ui;

    float meter_peak() const;
    float meter_rms() const;
    float meter_onset() const;
    
private:
	void freqLoChanged(float & pitch);
	void freqHiChanged(float & pitch);

    pdsp::AbsoluteValue     absolute;
    pdsp::EnvelopeFollower  envFollowerPeak;

    pdsp::RMSDetector       rms;
    pdsp::EnvelopeFollower  envFollowerRMS;

    pdsp::PositiveValue     difference;

    pdsp::HighCut           loCut;
    pdsp::LowCut            hiCut;

    pdsp::Parameter     hiFreqControl;
    pdsp::Parameter     rmsAttackControl;
    pdsp::Parameter     rmsReleaseControl;
    pdsp::Parameter     peakAttackControl;
    pdsp::Parameter     peakReleaseControl;

    pdsp::Amp        bandGain;
    pdsp::DBtoLin    bandFader;
    pdsp::Parameter  gainControl;            
    
    pdsp::PitchToFreq       p2fLo;
    pdsp::Parameter         loPitchControl;
    ofParameter<float>      loFreqValue;

    pdsp::PitchToFreq       p2fHi;
    pdsp::Parameter         hiPitchControl;
    ofParameter<float>      hiFreqValue;

    ofParameter<float>      onsetThreshold;
    
};

