
// ofxMidiInProcessor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDIINPROCESSOR_H_INCLUDED
#define OFXPDSPMIDI_MIDIINPROCESSOR_H_INCLUDED

#include "ofxMidi.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include "../DSP/pdspCore.h"

class ofxMidiInProcessor : public ofxMidiListener, public pdsp::Preparable {
    
public:
    ofxMidiInProcessor();
    ~ofxMidiInProcessor();
    
    void linkToMidiIn(ofxMidiIn &midiInput);
    void openPort(int index);
    void closePort();
    void listPorts();
        
    void processMidi( const int &bufferSize ) noexcept;
    
    void newMidiMessage(ofxMidiMessage& eventArgs) noexcept;
    	
    vector<_ofxPositionedMidiMessage>*   readVector;    	

protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;    
    
private: 

    ofxMidiIn   midiIn;
    ofxMidiIn*  midiIn_p;
    
    mutex       midiMutex;
    
    vector<_ofxPositionedMidiMessage>    midiMessageVectorA;
    vector<_ofxPositionedMidiMessage>    midiMessageVectorB;

    vector<_ofxPositionedMidiMessage>*   writeVector;

    double                                              oneSlashMicrosecForSample;
    chrono::time_point<chrono::high_resolution_clock>   bufferChrono;
    
    bool connected;
        
};


#endif // OFXPDSPMIDI_MIDIINPROCESSOR_H_INCLUDED
