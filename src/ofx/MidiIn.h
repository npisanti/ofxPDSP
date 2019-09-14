
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
    @brief open the port with the given name
    @param[in] name of the port to open
    */    
    void openPort( std::string name );

    /*!
    @brief create a virtual input port with the given name
    @param[in] name of the port to crate
    */    
    void openVirtualPort( std::string name );

    /*!
    @brief close the opened port
    */   
    void closePort();

    /*!
    @brief list the available ports
    */   
    void listPorts();

    /*!
    @brief return number of available midi in ports
    */
    int getPortCount();

    /*!
    @brief return list of available ports name
    */
    std::vector<std::string> getPortList();

    /*!
    @brief uses an already open ofxMidiIn instead of opening a port
    @param[in] midiInput ofxMidiIn object
    */    
    void linkToMidiIn(ofxMidiIn &midiInput);
       
       
/*!
    @cond HIDDEN_SYMBOLS
*/
    void processMidi( const int &bufferSize ) noexcept;
    
    void newMidiMessage(ofxMidiMessage& eventArgs) noexcept override;
    	
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
    
    std::atomic<int> index;
    int lastread;
    std::vector<_PositionedMidiMessage>    circularBuffer;
    std::vector<_PositionedMidiMessage>    readVector;

    double                                                      oneSlashMicrosecForSample;
    std::chrono::time_point<std::chrono::high_resolution_clock> bufferChrono;
    
    void pushToReadVector( _PositionedMidiMessage & message );
    
    bool connected;
    
    void initPort();
        
};

}}

#endif

#endif // OFXPDSPMIDI_PDSPMIDIIN_H_INCLUDED
