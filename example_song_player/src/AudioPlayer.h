
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

    
class AudioPlayer : public pdsp::Patchable {
    
public:
    AudioPlayer() { patch(); } 
    AudioPlayer( const AudioPlayer & other ) { patch(); } 
    
    ofParameterGroup    ui;
    
    void load(string path);
    void play();
    void pause();
    void stop();
    
    float playhead() const;
    
private:
    void patch ();

    void loadButtonCall( bool & value );
    void sampleChangedCall( string & value );
    void loadSample( string path );
    
    pdsp::Sampler       sampler0;
    pdsp::Sampler       sampler1;
    pdsp::ADSR          env;
    pdsp::Amp           amp0;
    pdsp::Amp           amp1;
    
    pdsp::Amp           fader0;
    pdsp::Amp           fader1;
    pdsp::DBtoLin       dBtoLin;
    pdsp::Parameter     faderControl;    

    pdsp::Parameter     pitchControl;
    pdsp::Parameter     smoothControl;

    pdsp::SampleBuffer  sample;    
    ofParameter<bool>   loadButton;
    ofParameter<string> samplePath;
    ofParameter<string> sampleName;

    ofParameter<bool>   bPlay;
    ofParameter<bool>   bPause;
    ofParameter<bool>   bStop;

    pdsp::TriggerControl    sampleTrig;
    pdsp::TriggerControl    envGate;
    
    static int number;

    void onPlay( bool & value );
    void onPause( bool & value );
    void onStop( bool & value );
    
    bool bSemaphore;
};   
    
