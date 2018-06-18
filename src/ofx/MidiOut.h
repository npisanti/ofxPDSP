
// Output.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED

#ifndef __ANDROID__

#include "ofxMidi.h"
#include <chrono>
#include "helper/PositionedMidiMessage.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/SequencerSection.h"

/*!
@brief utility class manage midi output ports and send midi messages from the internal generative music system
*/

namespace pdsp { namespace midi {

class Output : public pdsp::ExtSequencer, public pdsp::Preparable {

private:

    class ScheduledMidiMessage{
    public:
        ScheduledMidiMessage();
        ScheduledMidiMessage(ofxMidiMessage message, std::chrono::high_resolution_clock::time_point schedule);
        ScheduledMidiMessage(const ScheduledMidiMessage &other);
        ScheduledMidiMessage& operator= (const ScheduledMidiMessage &other);
        ~ScheduledMidiMessage();
        
        ofxMidiMessage                              midi;
        std::chrono::high_resolution_clock::time_point   scheduledTime;
    };
    
    
    static bool scheduledMidiSort(const ScheduledMidiMessage &lhs, const ScheduledMidiMessage &rhs );
    
    
public:
    Output();    
    ~Output();      
    
    /*!
    @brief open the port with the given index
    @param[in] index of the port to open
    */    
    void openPort(int index);

    /*!
    @brief close the opened port
    */   
    void close() override;

    /*!
    @brief list the available ports
    */   
    void listPorts();

    /*!
    @brief uses an already open ofxMidiOut instead of opening a port
    @param[in] midiOut ofxMidiOut object
    */    
    void linkToMidiOut(ofxMidiOut &midiOut);

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
    @brief patch a pdsp::ScoreSection::out_message() method to the result of this method for sending midi note message out
    
    see the example-midi_seq for a practical example
    */   
    pdsp::ExtSequencer& gate(int midiChannel, int defaultNote = 60);

    /*!
    @brief patch a pdsp::ScoreSection::out_message() method to the result of this method for controlling the midi note message out.
    
    Patch a pdsp::ScoreSection::out_message() method to the result of this method for controlling the midi note message out. The output controlled is the one of the last gate() call. See the example-midi_seq for a practical example.
    */   
    pdsp::ExtSequencer& note(); 

    /*!
    @brief patch a pdsp::ScoreSection::out_message() method to the result of this method for sending midi cc message out
    
    See the example-midi_seq for a practical example.
    */   
    pdsp::ExtSequencer& cc(int midiChannel, int ccNumber);

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

    ofxMidiOut*     midiOut_p;
    ofxMidiOut      midiOut;
    bool            connected;
    bool            verbose;
   
    std::vector<ScheduledMidiMessage> messagesToSend;
    
    std::vector<pdsp::MessageBuffer*>    inputs [3];
    std::vector<int>                     defaultNote;
    std::vector<int>                     midiChannelsNote;
    std::vector<int>                     ccNumbers;
    std::vector<int>                     midiChannelsCC;
    
    int     selectedType;
    int     selectedMidiChannel;
    int     selectedDefaultNote;
    int     selectedCC;
  
    static const int    nullType = -1;
    static const int    gateType = 0;
    static const int    noteType = 1;
    static const int    ccType = 2;    
    
    //MIDI DAEMON MEMBERS---------------------------------------------------------------
    void                                                startMidiDaemon();
    void                                                closeMidiDaemon();
    void                                                midiDaemonFunction() noexcept;
    static void                                         midiDaemonFunctionWrapper(Output* parent);
    
    std::thread                 midiDaemonThread;
    std::atomic<bool>           runMidiDaemon;
    
    //midi output processing members
    bool                                                        chronoStarted;
    std::chrono::time_point<std::chrono::high_resolution_clock> bufferChrono;     
    double                                                  usecPerSample;
    std::vector<ScheduledMidiMessage>                    circularBuffer;
   
    std::atomic<int>                                        writeindex;
    int                                                     send;


    //TESTING
    int messageCount;
};

}} // end namespaces

#endif

#endif //OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED
