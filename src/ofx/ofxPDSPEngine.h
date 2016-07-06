
// ofxPDSPEngine.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFX_PDSP_ENGINE_H_INCLUDED
#define OFX_PDSP_ENGINE_H_INCLUDED

#include "ofMain.h"
#include "../DSP/pdspCore.h"
#include "../sequencer/ScoreProcessor.h"
#include "ofxPDSPController.h"
#include "ofxPDSPMidiIn.h"
#include "ofxPDSPMidiOut.h"
#include "ofxPDSPSerialOut.h"


/*!
@brief utility class to manage input/output audio streams, acquire and release resources and process midi input/output. It also has an internal ScoreProcessor for sequencing.
*/
class ofxPDSPEngine : public ofBaseSoundInput, public ofBaseSoundOutput{
    
public:
    ofxPDSPEngine();
    ~ofxPDSPEngine();
    
    /*!
    @brief prepares all the module and units, initializing their internal buffers, then setup up the audioStreams and start the audio engine. Check oF audio tutorials for more info on the parameters.
    @param[in] sampleRate audio callbacks sample rate
    @param[in] bufferSize audio callbacks expected buffer size. 
    @param[in] nBuffers number of buffers in the audioQueue
    */
    void setup(int sampleRate, int bufferSize, int nBuffers);
    
    /*!
    @brief starts the audio streams again if they were stopped.
    */
    void start();
    
    /*!
    @brief stop the audio streams.
    */
    void stop();
    
    /*!
    @brief close everything. This is called automatically on exit and probably you should never use it.
    */
    void close();
    
    /*!
    @brief sets the number of inputs and output channels. If not called, the standard is 0 input channels and 2 output channels (L/R).
    @param[in] inputChannels input channels number
    @param[in] outputChannels output channels number
    */
    void setChannels( int inputChannels, int outputChannels );
    
    /*!
    @brief calls ofSoundStreamListDevices() for listing the available audio devices. 
    */
    std::vector<ofSoundDevice> listDevices();
    
    /*!
    @brief sets the ID of the audio device to open. You can get the available ID with listDevices(). Sometimes you have to use different ID for input and output devices, for that use setInputDeviceID() and setOutputDeviceID().
    @param[in] deviceID ID to set
    */
    void setDeviceID(int deviceID);
    
    /*!
    @brief sets the ID of the audio device to open for audio input.
    @param[in] deviceID ID to set
    */
    void setInputDeviceID(int deviceID);
    
    /*!
    @brief sets the ID of the audio device to open for audio output.
    @param[in] deviceID ID to set
    */
    void setOutputDeviceID(int deviceID);

    /*!
    @brief adds a midi controller and a relative midi input to the engine, making them active.
    @param[in] controller controller to add
    @param[in] midiIn midiIn for the controller.
    Adds a midi controller and a relative midi input to the engine, making them active. You can use the same ofxPDSPMidiIn for multiple controllers, that are parsing the midi data in different way, for example if you have a keyboard with some knob and faders you can use the same midiIn with a ofxPDSPMidiKeys for the key, pitch bend and pressure output and a ofxPDSPMidiControls for the knobs and faders.
    */
    void addMidiController( ofxPDSPController & controller, ofxPDSPMidiIn & midiIn );

    /*!
    @brief adds a midi output to the engine, making it active.
    @param[in] midiOut midi out object to activate
    */
    void addMidiOut( ofxPDSPMidiOut & midiOut );

    /*!
    @brief adds a serial output to the engine, making it active.
    @param[in] serialOut serial out object to activate
    */
    void addSerialOut( ofxPDSPSerialOut & serialOut );
    
    /*!
    @brief adds an external output to the engine, like an ofxPDSPSerialOut or an ofxPDSPMidiOut making it active.
    @param[in] externalOut external out object to activate
    */
    void addExternalOut( pdsp::ExtSequencer & externalOut );

    /*!
    @brief returns a Patchable object that rapresent the audio out of the system. Patch your module to this for connecting them to the selected device audio output.
    @param[in] channel channel to connect
    */
    pdsp::Patchable& audio_out( int channel );
    
    /*!
    @brief returns a Patchable object that rapresent the audio input of the system. Patch this to your modules for processing the selected device audio input.
    @param[in] channel channel to connect
    */
    pdsp::Patchable& audio_in(  int channel );

    /*!
    @brief a ScoreProcessor class to manage the playhead and for sequencing. Look ScoreProcessor page for knowing more.
    */
    pdsp::ScoreProcessor score;
    
    
private:
    ofSoundStream inputStream;
    ofSoundStream outputStream;

    bool inStreamActive;
    bool outStreamActive;    
    
    int inputID;
    int outputID;
    
    int outputChannels;
    int inputChannels;
        
    pdsp::Processor processor;
    std::vector<pdsp::ExternalInput> inputs;
    
    std::vector<ofxPDSPMidiIn*>         midiIns;
    std::vector<ofxPDSPController*>     controllers;
    std::vector<ofxPDSPMidiIn*>         controllerLinkedMidis;
    bool                                hasMidiIn;

    std::vector<pdsp::ExtSequencer*>          externalOuts;  
    bool                                hasExternalOut;
    
    int state;

    static const int closedState = 0;
    static const int stoppedState = 1;
    static const int startedState = 2;

    void audioOut(ofSoundBuffer &outBuffer);
    void audioIn (ofSoundBuffer &outBuffer);
    void onExit( ofEventArgs &args);
    
};

#endif // OFX_PDSP_ENGINE_H_INCLUDED
