
// SerialOut.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPSERIALOUT_H_INCLUDED
#define OFXPDSPMIDI_PDSPSERIALOUT_H_INCLUDED

#include "ofMain.h"

#ifndef TARGET_OF_IOS
#ifndef __ANDROID__

#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/SequencerSection.h"

/*!
@brief utility class manage serial output ports and send bytes from the internal generative music system
*/

namespace pdsp{ namespace serial {

class Output : public pdsp::ExtSequencer, public pdsp::Preparable {

private:

    /*!
        @cond HIDDEN_SYMBOLS
    */

    
    class ScheduledSerialMessage{
    public:
        ScheduledSerialMessage();
        ScheduledSerialMessage(int channel, float message, std::chrono::high_resolution_clock::time_point schedule);
        ScheduledSerialMessage(const ScheduledSerialMessage &other);
        ScheduledSerialMessage& operator= (const ScheduledSerialMessage &other);
        ~ScheduledSerialMessage();
        
        signed char channel;
        signed char message;
        std::chrono::high_resolution_clock::time_point   scheduledTime;
    };
    
    
    static bool scheduledSort(const ScheduledSerialMessage &lhs, const ScheduledSerialMessage &rhs );

    /*!
        @endcond
    */
    
    
public:
    Output();    
    ~Output();      
    
    /*!
    @brief open the port with the given index
    @param[in] index of the port to open
    @param[in] baudRate rate of the connection, 250000 baud if not given
    */    
    void openPort(int index, int baudRate=250000);

    /*!
    @brief open the port with the given index
    @param[in] name name of the serial output path
    @param[in] baudRate rate of the connection, 250000 baud if not given
    */    
    void openPort(string name, int baudRate=250000);

    /*!
    @brief close the opened port
    */   
    void close() override;

    /*!
    @brief list the available ports
    */   
    void listPorts();

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
    pdsp::ExtSequencer& channel(int channelNumber);

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

    ofSerial	    serial;
    
    bool            connected;
    bool            verbose;
   
    std::vector<ScheduledSerialMessage> messagesToSend;
    
    std:: vector<pdsp::MessageBuffer*>    inputs;
    std::vector<int>                     channels;
    
    int     selectedChannel;
    int     messageCount;


    //MIDI DAEMON MEMBERS---------------------------------------------------------------
    void                                                startDaemon();
    void                                                closeDaemon();
    void                                                daemonFunction() noexcept;
    static void                                         daemonFunctionWrapper(Output* parent);
    
    std::thread                                         daemonThread;
    std::atomic<bool>                                   runDaemon;
    
    //serial output processing members
    std::chrono::time_point<std::chrono::high_resolution_clock>   bufferChrono;     
    bool                                                chronoStarted;
    double                                              usecPerSample;
    std::vector<ScheduledSerialMessage>                 circularBuffer;
   
    std::atomic<int>                                    writeindex;
    int                                                 send;

};

}} // end namespaces

#endif // __ANDROID__
#endif // TARGET_OF_IOS

#endif //OFXPDSPMIDI_PDSPSERIALOUT_H_INCLUDED
