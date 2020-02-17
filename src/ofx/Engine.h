
// Engine.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef OFX_PDSP_ENGINE_H_INCLUDED
#define OFX_PDSP_ENGINE_H_INCLUDED

#include "ofMain.h"
#include "../DSP/pdspCore.h"
#include "../sequencer/SequencerSection.h"
#include "../sequencer/SequencerProcessor.h"
#include "../DSP/utility/OneBarTimeMs.h"
#include "../modules/oscillators/FMOperator.h"
#include "../DSP/utility/DBtoLin.h"

#include "SerialOut.h"
#include "OscInput.h"

#ifndef __ANDROID__
#include "helper/Controller.h"
#include "MidiIn.h"
#include "MidiOut.h"
#endif

#ifdef TARGET_OF_IOS
    #include "ofxiOSSoundStream.h"
#endif

/*!
@brief utility class to manage input/output audio streams, acquire and release resources and process midi input/output. It also has an internal SequencerProcessor for sequencing.
*/

namespace pdsp{

class Engine : public ofBaseSoundInput, public ofBaseSoundOutput{
    
public:
    Engine();
    ~Engine();
    
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
    @brief sets the API to use for the engine soundstream, for the APIs see oF `ofSoundBaseTypes.h` file.
    @param[in] api name of the API to use
    */
    void setApi( ofSoundDevice::Api api );
    
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

    [[deprecated("deprecated, handled automatically between construction and destruction")]]
    void addOscInput( pdsp::osc::Input & oscInput );

#ifndef __ANDROID__
    /*!
    @brief adds a midi controller and a relative midi input to the engine, making them active.
    @param[in] controller controller to add
    @param[in] midiIn midiIn for the controller.
    Adds a midi controller and a relative midi input to the engine, making them active. You can use the same pdsp::midi::Input for multiple controllers, that are parsing the midi data in different way, for example if you have a keyboard with some knob and faders you can use the same midiIn with a ofxPDSPMidiKeys for the key, pitch bend and pressure output and a ofxPDSPMidiControls for the knobs and faders.
    */
    void addMidiController( pdsp::Controller & controller, pdsp::midi::Input & midiIn );

    /*!
    @brief adds a midi output to the engine, making it active.
    @param[in] midiOut midi out object to activate
    */
    void addMidiOut( pdsp::midi::Output & midiOut );

#ifndef TARGET_OF_IOS
    /*!
    @brief adds a serial output to the engine, making it active.
    @param[in] serialOut serial out object to activate
    */
    void addSerialOut( pdsp::serial::Output & serialOut );
#endif // TARGET_OF_IOS
#endif // __ANDROID__
    
    /*!
    @brief adds an external output to the engine ( for example an pdsp::serial::Output or an pdsp::midi::Output ) making it active.
    @param[in] externalOut external out object to activate
    */
    void addExternalOut( pdsp::ExtSequencer & externalOut );

    /*!
    @brief returns a Patchable object that represent the audio out of the system. Patch your module to this for connecting them to the selected device audio output.
    @param[in] channel channel to connect
    */
    pdsp::Patchable& audio_out( int channel );
    
    /*!
    @brief returns a Patchable object that represent the audio input of the system. Patch this to your modules for processing the selected device audio input.
    @param[in] channel channel to connect
    */
    pdsp::Patchable& audio_in(  int channel );
    
    /*!
    @brief returns a Patchable object that continously process all output patched to it without outputting nothing. Patch your Units and modules to this channels if you need them to be always active for some reason;
    */    
    pdsp::Patchable& blackhole( );
    

    /*!
    @brief a SequencerProcessor class to manage the playhead and for sequencing. Look SequencerProcessor page for knowing more.
    */
    pdsp::SequencerProcessor sequencer;
   
    /*!
    @brief returns a module that outputs the time of a musical bar in milliseconds
    */    
    pdsp::Patchable & out_bar_ms();
           
    /*!
    @brief activate/deactivate a sine tone patched to the left output to testing that the engine is running.
    @param testingActive pass true to activate testing, false to deactivate
    @param testingDB optional volume of testing tone, -12dB if not given
    */       
    void test( bool testingActive, float testingDB=-12.0f );

    /*!
    @brief activate/deactivate background audio on ios, this function has to be called before the setup.
    @param mix true to activate background audio, false to deactivate, false is default
    */   
    void setBackgroundAudio( bool active );

/*!
    @cond HIDDEN_SYMBOLS
*/
    void audioOut(ofSoundBuffer &outBuffer);
    void audioIn (ofSoundBuffer &outBuffer);
    
    pdsp::SequencerProcessor & score; // this is an alias for the sequencer, legacy reasons

/*!
    @endcond
*/

private:

    ofSoundStream inputStream;
    ofSoundStream outputStream;
    ofSoundDevice::Api api;
    
    bool inStreamActive;
    bool outStreamActive;    
    
    int inputID;
    int outputID;
    
    int outputChannels;
    int inputChannels;
        
    pdsp::Processor processor;
    std::vector<pdsp::ExternalInput> inputs;

#ifndef __ANDROID__
    std::vector<pdsp::midi::Input*> midiIns;
    std::vector<pdsp::Controller*>  controllers;
    std::vector<pdsp::midi::Input*> controllerLinkedMidis;
    bool                            hasMidiIn;
#endif

    std::vector<pdsp::ExtSequencer*>    externalOuts;  
    bool                                hasExternalOut;
    
    int state;

    static const int closedState = 0;
    static const int stoppedState = 1;
    static const int startedState = 2;


    void onExit( ofEventArgs &args);
    
    pdsp::FMOperator testOscillator;
    pdsp::Amp        testAmp;
    pdsp::OneBarTimeMs barTime;
    
    bool bBackgroundAudio;
        
};

}

#endif // OFX_PDSP_ENGINE_H_INCLUDED
