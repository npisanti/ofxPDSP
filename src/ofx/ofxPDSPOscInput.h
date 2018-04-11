
// ofxPDSPOscInput.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPOSCINPUT_H_INCLUDED
#define OFXPDSPMIDI_PDSPOSCINPUT_H_INCLUDED

#include "ofMain.h"
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/ScoreSection.h"
#include "ofxOsc.h"

/*!
@brief utility class manage OSC input to the DSP
*/
class ofxPDSPOscInput : public pdsp::Preparable {

private:

    //enum OscChannelMode { Gate, Value };

    class OscChannel {
    public:
        OscChannel();
        
        void deallocate();
        
        string key;
        pdsp::MessageBuffer* messageBuffer;
        //OscChannelMode mode;
        pdsp::GateSequencer* gate_out;
        pdsp::ValueSequencer* value_out;
        
    };
    
    class _ofxPositionedOscMessage {
    public:
        _ofxPositionedOscMessage(){ sample = -1; };
        _ofxPositionedOscMessage(ofxOscMessage message, int sample) : message(message), sample(sample){};
        
        ofxOscMessage message;
        int sample;
    };


public:
    ofxPDSPOscInput();    
    ~ofxPDSPOscInput();      
    

    /*!
    @brief open the port with the given index
    @param[in] port OSC port
    */    
    void openPort( int port );

    /*!
    @brief shuts down the output
    */   
    void close();


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
    @brief get a trigger output for the given OSC address. Only the first value of those address will be taken, as float value. When you have used an address as trig output you can't use it as value
    @param[in] oscAddress address for OSC input
    */       
    pdsp::GateSequencer& out_trig( string oscAddress );

    /*!
    @brief get a value output for the given OSC address. Only the first value of those address will be taken, as float value. When you have used an address as value output you can't use it as trigger
    @param[in] oscAddress address for OSC input
    */    
    pdsp::ValueSequencer& out_value( string oscAddress );


    /*!
    @brief sends 0.0f as message to all the connected trigger and value outputs.
    */   
    void clearAll();

/*!
    @cond HIDDEN_SYMBOLS
*/  
    void processOsc( int bufferSize ) noexcept;
/*!
    @endcond
*/   

protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;    

private:
    ofxOscReceiver  receiver;
        
    vector<OscChannel*> oscChannels;    
    
    bool            connected;
    bool            verbose;    
    
    bool    sendClearMessages; 

    mutex       oscMutex;
    
    vector<_ofxPositionedOscMessage>*   readVector;       
    vector<_ofxPositionedOscMessage>    oscMessageVectorA;
    vector<_ofxPositionedOscMessage>    oscMessageVectorB;

    vector<_ofxPositionedOscMessage>*   writeVector;

    double                                              oneSlashMicrosecForSample;

    
    //bool                                                chronoStarted;
    chrono::time_point<chrono::high_resolution_clock>   bufferChrono;     
    //double                                              usecPerSample;

    pdsp::GateSequencer invalidGate;
    pdsp::ValueSequencer invalidValue;


    void                                                startDaemon();
    void                                                closeDaemon();
    void                                                daemonFunction() noexcept;
    static void                                         daemonFunctionWrapper(ofxPDSPOscInput* parent);
    thread                                              daemonThread;
    atomic<bool>                                        runDaemon;
    int                                                 daemonRefreshRate;

};






#endif // OFXPDSPMIDI_PDSPOSCINPUT_H_INCLUDED
