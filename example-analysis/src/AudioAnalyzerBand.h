
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


    ofxPDSPValue     hiFreqControl;
    ofxPDSPValue     rmsAttackControl;
    ofxPDSPValue     rmsReleaseControl;
    ofxPDSPValue     peakAttackControl;
    ofxPDSPValue     peakReleaseControl;

    pdsp::Amp        bandGain;
    pdsp::DBtoLin    bandFader;
    ofxPDSPValue     gainControl;            
    
    pdsp::PitchToFreq       p2fLo;
    ofxPDSPValue            loPitchControl;
    ofParameter<float>      loFreqValue;

    pdsp::PitchToFreq       p2fHi;
    ofxPDSPValue            hiPitchControl;
    ofParameter<float>      hiFreqValue;

    ofParameter<float>      onsetThreshold;
    
};

