
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
        ScheduledOscMessage(ofxOscMessage message, chrono::high_resolution_clock::time_point schedule);
        ScheduledOscMessage(const ScheduledOscMessage &other);
        ScheduledOscMessage& operator= (const ScheduledOscMessage &other);
        ~ScheduledOscMessage();
        
        ofxOscMessage message;
        chrono::high_resolution_clock::time_point   scheduledTime;
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
    void openPort(const string &hostname, int port );

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
    pdsp::ExtSequencer& address( string oscAddress );

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
    vector<string>                  addresses;
    string                          selectedAddress;
    
        
    bool            connected;
    bool            verbose;
   
    vector<ScheduledOscMessage> messagesToSend;
    
    vector<pdsp::MessageBuffer*>    inputs;

    int     messageCount;


    //MIDI DAEMON MEMBERS---------------------------------------------------------------
    void                                                startDaemon();
    void                                                prepareForDaemonAndNotify();
    void                                                closeDaemon();
    void                                                daemonFunction() noexcept;
    static void                                         daemonFunctionWrapper(Output* parent);
    
    thread                                              daemonThread;
    mutex                                               outMutex;
    condition_variable                                  outCondition;
    atomic<bool>                                        messagesReady;
    atomic<bool>                                        runDaemon;
    
    //serial output processing members
    bool                                                chronoStarted;
    chrono::time_point<chrono::high_resolution_clock>   bufferChrono;     
    double                                              usecPerSample;
    vector<ScheduledOscMessage>                         circularBuffer;
    int                                                 circularRead;
    int                                                 circularWrite;
    int                                                 circularMax;

};

}}

#endif //OFXPDSPMIDI_PDSPOSCOUT_H_INCLUDED
