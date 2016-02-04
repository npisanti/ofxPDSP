
// ofxMidiOutProcessor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDIOUTPUTPROCESSOR_H_INCLUDED
#define OFXPDSPMIDI_MIDIOUTPUTPROCESSOR_H_INCLUDED

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


class ofxMidiOutProcessor : public pdsp::ExtSequencer, public pdsp::Preparable {

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
    ofxMidiOutProcessor();    
    ~ofxMidiOutProcessor();      
    
    void linkToMidiOut(ofxMidiOut &midiOut);
    void listPorts();
    void openPort(int portIndex);
    void closePort();
    
    bool isConnected() { return connected; }
    
    void setVerbose( bool verbose );
    
    void process() noexcept;


    pdsp::ExtSequencer& gate(int midiChannel, int defaultNote = 60);
    pdsp::ExtSequencer& note(); //link to the last gate 
    pdsp::ExtSequencer& cc(int midiChannel, int ccNumber);
  
    void linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) override;
    void unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) override;
    
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
    static void                                         midiDaemonFunctionWrapper(ofxMidiOutProcessor* parent);
    
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






#endif //OFXPDSPMIDI_MIDIOUTPUTPROCESSOR_H_INCLUDED
