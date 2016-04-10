
// ofxPDSPMidiIn.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED

#include "ofxMidi.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include "../DSP/pdspCore.h"

/*!
@brief utility class manage midi input ports and collect midi input messages
*/
class ofxPDSPMidiIn : public ofxMidiListener, public pdsp::Preparable {
    
public:
    ofxPDSPMidiIn();
    ~ofxPDSPMidiIn();
    

    /*!
    @brief open the port with the given index
    @param[in] index of the port to open
    */    
    void openPort(int index);

    /*!
    @brief close the opened port
    */   
    void closePort();

    /*!
    @brief list the available ports
    */   
    void listPorts();

    /*!
    @brief uses an already open ofxMidiIn instead of opening a port
    @param[in] midiInput ofxMidiIn object
    */    
    void linkToMidiIn(ofxMidiIn &midiInput);
       
       
/*!
    @cond HIDDEN_SYMBOLS
*/
    void processMidi( const int &bufferSize ) noexcept;
    
    void newMidiMessage(ofxMidiMessage& eventArgs) noexcept;
    	
 	const vector<_ofxPositionedMidiMessage> & getMessageVector() const;
/*!
    @endcond
*/

protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;    
    
private: 

    ofxMidiIn   midiIn;
    ofxMidiIn*  midiIn_p;
    
    mutex       midiMutex;
    
    vector<_ofxPositionedMidiMessage>*   readVector;       
    vector<_ofxPositionedMidiMessage>    midiMessageVectorA;
    vector<_ofxPositionedMidiMessage>    midiMessageVectorB;

    vector<_ofxPositionedMidiMessage>*   writeVector;

    double                                              oneSlashMicrosecForSample;
    chrono::time_point<chrono::high_resolution_clock>   bufferChrono;
    
    bool connected;
        
};


#endif // OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED
