
// OscOutput.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPOSCOUT_H_INCLUDED
#define OFXPDSPMIDI_PDSPOSCOUT_H_INCLUDED

#include "ofMain.h"
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/SequencerSection.h"
#include "ofxOsc.h"

/*!
@brief utility class manage OSC output from the sequencer
*/

namespace pdsp { namespace osc {

class Output : public pdsp::ExtSequencer, public pdsp::Preparable {

private:

    /*!
        @cond HIDDEN_SYMBOLS
    */

    
    class ScheduledOscMessage{
    public:
        ScheduledOscMessage();
        ScheduledOscMessage(ofxOscMessage message, std::chrono::high_resolution_clock::time_point schedule);
        ScheduledOscMessage(const ScheduledOscMessage &other);
        ScheduledOscMessage& operator= (const ScheduledOscMessage &other);
        ~ScheduledOscMessage();
        
        ofxOscMessage message;
		std::chrono::high_resolution_clock::time_point   scheduledTime;
    };
    
    
    static bool scheduledSort(const ScheduledOscMessage &lhs, const ScheduledOscMessage &rhs );

    /*!
        @endcond
    */
    
    
public:
    Output();    
    ~Output();      
    

    /*!
    @brief open the port with the given index
    @param[in] hostname OSC hostname
    @param[in] port OSC port
    */    
    void openPort(const std::string &hostname, int port );

    /*!
    @brief shuts down the output
    */   
    void close() override;



    /*!
    @brief return true if the port has been sucessfully opened
    */   
    bool isConnected() { return connected; }
    
    /*!
    @brief enable or disable diagnostic messages
    @param[in] verbose true for enabling, false for disabling
    */   
    void setVerbose( bool verbose );


    /*!
    @brief patch a pdsp::ScoreSection::out_message() method to the result of this method for message to the serial out
    
    */   
    pdsp::ExtSequencer& address( std::string oscAddress );

/*!
    @cond HIDDEN_SYMBOLS
*/  
    void process( int bufferSize ) noexcept override;

    void linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) override;
    void unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) override;
/*!
    @endcond
*/    
protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;

private:

    ofxOscSender	sender;
    std::vector<std::string>    addresses;
    std::string                 selectedAddress;
    
        
    bool            connected;
    bool            verbose;
   
    
    std::vector<pdsp::MessageBuffer*>    inputs;

    std::vector<ScheduledOscMessage> messagesToSend;
    int     messageCount;


    //MIDI DAEMON MEMBERS---------------------------------------------------------------
    void                                                startDaemon();
    void                                                closeDaemon();
    void                                                daemonFunction() noexcept;
    static void                                         daemonFunctionWrapper(Output* parent);
    
	std::thread                                              daemonThread;
    std::atomic<bool>                                   runDaemon;
    
    //serial output processing members
    std::chrono::time_point<std::chrono::high_resolution_clock>   bufferChrono;   
    bool                                                chronoStarted;
    double                                              usecPerSample;
    std::vector<ScheduledOscMessage>                    circularBuffer;
   
    std::atomic<int>                                    writeindex;
    int                                                 send;

};

}}

#endif //OFXPDSPMIDI_PDSPOSCOUT_H_INCLUDED
