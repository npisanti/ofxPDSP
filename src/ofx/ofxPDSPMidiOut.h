
// ofxPDSPMidiOut.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED

#include "ofxMidi.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/ScoreSection.h"

/*!
@brief utility class manage midi output ports and send midi messages from the internal generative music system
*/
class ofxPDSPMidiOut : public pdsp::ExtSequencer, public pdsp::Preparable {

private:

    class ofxScheduledMidiMessage{
    public:
        ofxScheduledMidiMessage();
        ofxScheduledMidiMessage(ofxMidiMessage message, chrono::high_resolution_clock::time_point schedule);
        ofxScheduledMidiMessage(const ofxScheduledMidiMessage &other);
        ofxScheduledMidiMessage& operator= (const ofxScheduledMidiMessage &other);
        ~ofxScheduledMidiMessage();
        
        ofxMidiMessage                              midi;
        chrono::high_resolution_clock::time_point   scheduledTime;
    };
    
    
    static bool scheduledMidiSort(const ofxScheduledMidiMessage &lhs, const ofxScheduledMidiMessage &rhs );
    
    
public:
    ofxPDSPMidiOut();    
    ~ofxPDSPMidiOut();      
    
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
    void process() noexcept;

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
   
    vector<ofxScheduledMidiMessage> messagesToSend;
    
    vector<pdsp::MessageBuffer*>    inputs [3];
    vector<int>                     defaultNote;
    vector<int>                     midiChannelsNote;
    vector<int>                     ccNumbers;
    vector<int>                     midiChannelsCC;
    
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
    void                                                prepareForDaemonAndNotify();
    void                                                closeMidiDaemon();
    void                                                midiDaemonFunction() noexcept;
    static void                                         midiDaemonFunctionWrapper(ofxPDSPMidiOut* parent);
    
    thread                                              midiDaemonThread;
    mutex                                               midiOutMutex;
    condition_variable                                  midiOutCondition;
    atomic<bool>                                        messagesReady;
    atomic<bool>                                        runMidiDaemon;
    
    //midi output processing members
    chrono::time_point<chrono::high_resolution_clock>   bufferChrono;     
    double                                              usecPerSample;
    vector<ofxScheduledMidiMessage>                     circularBuffer;
    int                                                 circularRead;
    int                                                 circularWrite;
    int                                                 circularMax;

    //TESTING
    int messageCount;
};


#endif //OFXPDSPMIDI_PDSPMIDIOUT_H_INCLUDED
