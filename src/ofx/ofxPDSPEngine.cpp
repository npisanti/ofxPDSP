
#include "ofxPDSPEngine.h"


ofxPDSPEngine::ofxPDSPEngine(){
    inputID = 0;
    outputID = 0;
    inStreamActive = false;
    outStreamActive = false;
    
    inputChannels = outputChannels = -1; // for the next method to work
    setChannels(0, 2);
    
    state = closedState;

    externalOuts.reserve(10);
    hasExternalOut = false;
    hasOscIn = false;

#ifndef __ANDROID__
    midiIns.reserve(10);
    controllers.reserve(20);
    controllerLinkedMidis.reserve(20);

    midiIns.clear();
    controllers.clear();
    hasMidiIn = false;
#endif

    bBackgroundAudio = false;
    graphics.setParent( score );

    ofAddListener( ofEvents().exit, this, &ofxPDSPEngine::onExit );
}


ofxPDSPEngine::~ofxPDSPEngine(){
    if(state!= closedState){
        close();
    }
}

void ofxPDSPEngine::onExit( ofEventArgs &args ){
    if(state!= closedState){
        close();
    }
}

std::vector<ofSoundDevice> ofxPDSPEngine::listDevices(){
    return ofSoundStreamListDevices();
}

void ofxPDSPEngine::setDeviceID(int deviceID){
    outputID = deviceID;
    inputID = deviceID;
}

void ofxPDSPEngine::setOutputDeviceID(int deviceID){
    outputID = deviceID;
}

void ofxPDSPEngine::setInputDeviceID(int deviceID){
    inputID = deviceID;
}

pdsp::Patchable& ofxPDSPEngine::audio_out( int channel ){
    
    if(channel < 0 ) channel = 0;
    int maxChannels = processor.channels.size();
  
    if(channel >= maxChannels){
        cout<<"[pdsp] warning! requested audio output channel out of range, clamping to max\n";
        #ifdef NDEBUG
        std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
        #endif 
        assert(false);
        channel = maxChannels-1;
    }
    
    return processor.channels[channel];
    
}

pdsp::Patchable& ofxPDSPEngine::audio_in( int channel ){
    
    if(channel < 0 ) channel = 0;
    int maxChannels = inputs.size();
  
    if(channel >= maxChannels){
        cout<<"[pdsp] warning! requested audio input channel out of range, clamping to max\n";
        #ifdef NDEBUG
        std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
        #endif 
        assert(false);
        channel = maxChannels-1;
    }
    
    return inputs[channel].out_signal();
    
}

pdsp::Patchable& ofxPDSPEngine::blackhole( ) {
    return processor.blackhole;
}

void ofxPDSPEngine::setChannels( int inputChannels, int outputChannels ){
    if(this->inputChannels != inputChannels){
        this->inputChannels = inputChannels;
        inputs.resize(inputChannels);   
    }
    
    if(this->outputChannels != outputChannels){
        this->outputChannels = outputChannels;
        processor.resize(outputChannels);
    }

}

void ofxPDSPEngine::setup( int sampleRate, int bufferSize, int nBuffers){
 
    ofLogNotice()<<"[pdsp] engine: starting with parameters: buffer size = "<<bufferSize<<" | sample rate = "<<sampleRate<<" | "<<inputChannels<<" inputs | "<<outputChannels<<" outputs\n";
   
    if ( nBuffers < 1 ) nBuffers = 1;
    
    // prepare all the units / modules
    pdsp::prepareAllToPlay(bufferSize, static_cast<double>(sampleRate) );


    // starts engine
    
#if (OF_VERSION_MINOR <= 9) // OF LEGACY for 0.9.8
    if(outputChannels > 0 && inputChannels == 0){
        outputStream.setOutput(*this);   
        outStreamActive = true; 
        outputStream.setDeviceID( outputID );
        outputStream.setup(outputChannels, 0, sampleRate, bufferSize, nBuffers);
    
    }else if(inputChannels > 0 && outputChannels == 0){
        inputStream.setInput(*this);  
        inStreamActive = true;
        inputStream.setDeviceID( inputID );
        inputStream.setup(0, inputChannels, sampleRate, bufferSize, nBuffers);
        
    }else{
        if( inputID == outputID ) {
            outputStream.setInput(*this);  
            outputStream.setOutput(*this);  
            outStreamActive = true;
            outputStream.setDeviceID( outputID );
            outputStream.setup(outputChannels, inputChannels, sampleRate, bufferSize, nBuffers);
        } else {
            inputStream.setInput(*this);  
            outputStream.setOutput(*this);  
            outStreamActive = true;
            inStreamActive  = true;
            outputStream.setDeviceID( outputID );
            outputStream.setup(outputChannels, 0, sampleRate, bufferSize, nBuffers);
            inputStream.setDeviceID( inputID );
            inputStream.setup(0, inputChannels, sampleRate, bufferSize, nBuffers);
        }     
    }
#else // END OF LEGACY 0.9.8

    // OF MASTER VERSION
    
	ofSoundStreamSettings settings;
	settings.sampleRate = (size_t)  sampleRate;
	settings.bufferSize = (size_t)  bufferSize;
    settings.numBuffers = (size_t)  nBuffers;
	settings.numOutputChannels = (size_t) outputChannels;
	settings.numInputChannels = (size_t)  inputChannels;
    
    #if defined(__ANDROID__)
        if( outputChannels > 0 ){
            outStreamActive = true;
            outputStream.setOutput(this);
        }
        if(inputChannels > 0 ){
            outStreamActive = true;
            outputStream.setInput(this);
        }
        outputStream.setup( settings ); 

    #elif defined(TARGET_OF_IOS)
        if( outputChannels > 0 ){
            settings.setOutListener(this);
        }
        if(inputChannels > 0 ){
            settings.setInListener(this);
        }

        if( bBackgroundAudio ){ ofxiOSSoundStream::setMixWithOtherApps(true); }
        ofSoundStreamSetup( settings );

    #else
        auto devices = outputStream.getDeviceList();
        
        if( outputChannels > 0 ){
            outStreamActive = true;
            settings.setOutListener(this);
            settings.setOutDevice( devices[outputID] );
        }
        if(inputChannels > 0 ){
            outStreamActive = true;
            settings.setInListener(this);
            settings.setInDevice( devices[inputID] );
        }
        
        outputStream.setup( settings );

    #endif

#endif // END OF MASTER VERSION

    if( outputChannels > 0 ){
        84.0f >> testOscillator.in_pitch();
        testOscillator >> testAmp >> processor.channels[0];
    }

    state = startedState;

    #ifndef OF_TARGET_IOS
    ofLogNotice()<<"[pdsp] engine: started | buffer size = "<<outputStream.getBufferSize()<<" | sample rate = "<<outputStream.getSampleRate()<<" | "<<outputStream.getNumInputChannels()<<" inputs | "<<outputStream.getNumOutputChannels()<<" outputs\n";
    #endif
    
}

void ofxPDSPEngine::start(){
    if(inStreamActive && state < startedState){
        inputStream.start();
    }
    if(outStreamActive && state < startedState){
        outputStream.start();
    }
    #ifdef TARGET_IOS	
    ofSoundStreamStart();
    #endif    
    state = startedState;
}

void ofxPDSPEngine::stop(){
    if(state==startedState){
        if( inStreamActive ){
            inputStream.stop();
        }
        if( outStreamActive ){
            outputStream.stop();
        }    
        #ifdef TARGET_IOS	
        ofSoundStreamStop();	
        #endif    
    }
}

void ofxPDSPEngine::close(){
    
    cout<<"[pdsp] engine: closing...\n";
    
    stop();

#ifndef __ANDROID__
    if(hasMidiIn){
        for( ofxPDSPMidiIn * &in : midiIns ){
            in->closePort();
        } 
    }
#endif

    if(hasOscIn){
        for( ofxPDSPOscInput * &in : oscIns){
            in->close();
        } 
    }
    
    if(hasExternalOut){
        for( pdsp::ExtSequencer * &out : externalOuts ){
            out->close();
        } 
    }
    if( inStreamActive ){
        inputStream.close();
    }
    if( outStreamActive ){
        outputStream.close();
    }
    
    #ifdef TARGET_IOS	
    ofSoundStreamClose();
    #endif  
    pdsp::releaseAll();
    
    state = closedState;
    cout<<"[pdsp] engine: audio streams closed and resources released\n";    

}

void ofxPDSPEngine::audioOut(ofSoundBuffer &outBuffer) {
    
    int bufferSize =  outBuffer.getNumFrames() ;
    // score and playhead processing
    score.process( bufferSize );

#ifndef __ANDROID__
    // midi input processing
    if(hasMidiIn){
        for( ofxPDSPMidiIn * &in : midiIns){
            in->processMidi( bufferSize );
        } 
        for(int i=0; i<(int)controllers.size(); ++i){
            controllers[i]->processMidi( *(controllerLinkedMidis[i]), bufferSize );
        }
    }
#endif

    if(hasOscIn){
        for( ofxPDSPOscInput * &in : oscIns){
            in->processOsc( bufferSize );
        } 
    }
    
    // external outputs processing
    if(hasExternalOut){
        for( pdsp::ExtSequencer * &out : externalOuts){
            out->process( bufferSize );
        }
    }
    //DSP processing
    processor.processAndCopyInterleaved(outBuffer.getBuffer().data(), outBuffer.getNumChannels(), outBuffer.getNumFrames());    
}

void ofxPDSPEngine::audioIn (ofSoundBuffer &inBuffer) {
    for( int i=0; i<inputChannels; i++){
        inputs[i].copyInterleavedInput( inBuffer.getBuffer().data(), i,  inBuffer.getNumChannels(), inBuffer.getNumFrames());
    }
}

#ifndef __ANDROID__
void ofxPDSPEngine::addMidiController( ofxPDSPController & controller, ofxPDSPMidiIn & midiIn ){
    
    bool midiInFound = false;
    for( ofxPDSPMidiIn * &ptr : midiIns ){
        if( ptr == &midiIn ){
            midiInFound = true;
        } 
    }
    if( ! midiInFound ){
        midiIns.push_back( &midiIn );
    }
    
    bool midiControllerFound = false;
    
    for( ofxPDSPController * &ptr : controllers ){
        if( ptr == &controller ){
            midiControllerFound = true;
            cout<<"[pdsp] warning! you have already added this controller, you shouldn't add it twice\n";
            pdsp::pdsp_trace();
            assert(false);
        } 
    }
    if( ! midiControllerFound ){
        controllers.push_back( &controller );
        controllerLinkedMidis.push_back( &midiIn );
    }    
    hasMidiIn = true;
    
}

void  ofxPDSPEngine::addMidiOut( ofxPDSPMidiOut & midiOut ){
    addExternalOut( midiOut );
}
#endif

#ifndef TARGET_OF_IOS
#ifndef __ANDROID__
void ofxPDSPEngine::addSerialOut( ofxPDSPSerialOut & serialOut ) {
    addExternalOut( serialOut ); 
}
#endif
#endif

void ofxPDSPEngine::addExternalOut( pdsp::ExtSequencer & externalOut ) {
   
    bool externalOutFound = false;
    for( pdsp::ExtSequencer * &ptr : externalOuts ){
        if( ptr == &externalOut ){
            externalOutFound = true;
            cout<<"[pdsp] warning! you have already added this external output to the engine, you shouldn't add it twice\n";
            pdsp::pdsp_trace();
        } 
    }
    if( ! externalOutFound ){
        externalOuts.push_back( &externalOut );
    }
    hasExternalOut = true;
}

void ofxPDSPEngine::addOscInput( ofxPDSPOscInput & oscInput ) {
    bool oscInputFound = false;
    for( ofxPDSPOscInput * &ptr : oscIns ){
        if( ptr == &oscInput ){
            oscInputFound = true;
            cout<<"[pdsp] warning! you have already added this OSC input to the engine, you shouldn't add it twice\n";
            pdsp::pdsp_trace();
        } 
    }
    if( ! oscInputFound ){
        oscIns.push_back( &oscInput );
    }
    hasOscIn = true;    
}

void ofxPDSPEngine::test( bool testingActive, float testingDB ){
    if( testingActive ){
        dB(testingDB) >> testAmp.in_mod();
    }else{
        0.0f >> testAmp.in_mod();
    }
}

pdsp::Patchable & ofxPDSPEngine::out_bar_ms(){
    return barTime.out_signal();
}

void ofxPDSPEngine::setBackgroundAudio( bool active ){
    bBackgroundAudio = active;
}
