
// Input.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"
#include <chrono>
#include "helper/PositionedMidiMessage.h"
#include "../DSP/pdspCore.h"

/*!
@brief utility class manage midi input ports and collect midi input messages
*/

namespace pdsp { namespace midi {
    
class Input : public ofxMidiListener, public pdsp::Preparable {
    
public:
    Input();
    ~Input();
    

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
    	
 	const std::vector<_PositionedMidiMessage> & getMessageVector() const;
/*!
    @endcond
*/

protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;    
    
private: 

    ofxMidiIn   midiIn;
    ofxMidiIn*  midiIn_p;
    
    std::mutex midiMutex;
    
    std::vector<_PositionedMidiMessage>*   readVector;       
    std::vector<_PositionedMidiMessage>    midiMessageVectorA;
    std::vector<_PositionedMidiMessage>    midiMessageVectorB;

    std::vector<_PositionedMidiMessage>*   writeVector;

    double                                                      oneSlashMicrosecForSample;
    std::chrono::time_point<std::chrono::high_resolution_clock> bufferChrono;
    
    bool connected;
        
};

}}

#endif

#endif // OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED
