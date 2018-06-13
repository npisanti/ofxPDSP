
#include "AudioAnalyzerBand.h"

float AudioAnalyzerBand::meter_peak() const { 
    return envFollowerPeak.meter_output(); 
}

float AudioAnalyzerBand::meter_rms() const { 
    return envFollowerRMS.meter_output(); 
}

float AudioAnalyzerBand::meter_onset() const {         
    
    // the onset threshold will control how much a signal must surpass the average value to be considered a note onset
    if( difference.meter_output() > onsetThreshold){
        return envFollowerPeak.meter_output(); 
    }else{
        return 0.0f;
    }
}


void AudioAnalyzerBand::setup( ofxPDSPEngine & engine, string name, int input ){
    
                                        // before patching signals to an envelope follower you need to pass it to a detector
                                        // the choices are: pdsp::AbsoluteValue, pdsp::PositiveValue or pdsp::RMSDetector
                                        // patching to the blackhole process the modules without outputting sound
    engine.audio_in(input) >> bandGain >> loCut >> hiCut >> rms      >> envFollowerRMS  >> engine.blackhole();
                                                   hiCut >> absolute >> envFollowerPeak >> engine.blackhole();
                                                            // an envelope follower smoothes the signal 
                                                            // with a settable in_attack() and in_release() values
    
    // difference sum the peak signal and the inverted rms signal and output values if the result is positive
    // in this way difference could be a better choice for detecting note onsets
    envFollowerPeak        >> difference >> engine.blackhole();
    envFollowerRMS * -1.0f >> difference;         
    
    gainControl >> bandFader >> bandGain.in_mod();    

    // setting up controls and analyzerGui ---------------------
    loPitchControl >> p2fLo >> hiCut.in_freq();              
    hiPitchControl >> p2fHi >> loCut.in_freq();              
    
    rmsAttackControl   >> envFollowerRMS.in_attack();
    rmsReleaseControl  >> envFollowerRMS.in_release();
    peakAttackControl  >> envFollowerPeak.in_attack();
    peakReleaseControl >> envFollowerPeak.in_release();
        
    ui.setName( name );
    ui.add( gainControl.set( "band input gain", 0, -48, 24 ));  
    ui.add( loPitchControl.set( "band low", 43.0f, 20.0f, 136.0f ));  
    ui.add( loFreqValue.set(    "band lo hz", pdsp::p2f(43.0f), 25000.0f, 26000.0f ));  
    ui.add( hiPitchControl.set( "band hi", 100.0f, 20.0f, 136.0f ));  
    ui.add( hiFreqValue.set(    "band hi hz", pdsp::p2f(100.0f), 25000.0f, 26000.0f ));  
    ui.add( rmsAttackControl.set( "rms env attack ms", 50, 20, 250 ));  
    ui.add( rmsReleaseControl.set( "rms env release ms", 100, 20, 500 ));  
    ui.add( peakAttackControl.set( "peak env attack ms", 5, 1, 25 ));
    ui.add( peakReleaseControl.set( "peak env release ms", 10, 1, 50 ));
    ui.add( onsetThreshold.set("onset threshold", 0.05f, 0.01f, 1.0f) ); //controls how much a signal must surpass the average value to be considered a note onset

    // you need to add this listeners to update the hertz labels
    loPitchControl.getOFParameterFloat().addListener(this, &AudioAnalyzerBand::freqLoChanged);
    hiPitchControl.getOFParameterFloat().addListener(this, &AudioAnalyzerBand::freqHiChanged);
}

void AudioAnalyzerBand::freqLoChanged(float & pitch) {
    loFreqValue = pdsp::p2f(pitch);
}

void AudioAnalyzerBand::freqHiChanged(float & pitch) {
    hiFreqValue = pdsp::p2f(pitch);    
}
