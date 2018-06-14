
#include "Engine.h"


pdsp::Engine::Engine() : score( sequencer ){
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

    ofAddListener( ofEvents().exit, this, &pdsp::Engine::onExit );
}


pdsp::Engine::~Engine(){
    if(state!= closedState){
        close();
    }
}

void pdsp::Engine::onExit( ofEventArgs &args ){
    if(state!= closedState){
        close();
    }
}

std::vector<ofSoundDevice> pdsp::Engine::listDevices(){
    return ofSoundStreamListDevices();
}

void pdsp::Engine::setDeviceID(int deviceID){
    outputID = deviceID;
    inputID = deviceID;
}

void pdsp::Engine::setOutputDeviceID(int deviceID){
    outputID = deviceID;
}

void pdsp::Engine::setInputDeviceID(int deviceID){
    inputID = deviceID;
}

pdsp::Patchable& pdsp::Engine::audio_out( int channel ){
    
    if(channel < 0 ) channel = 0;
    int maxChannels = processor.channels.size();
  
    if(channel >= maxChannels){
        std::cout<<"[pdsp] warning! requested audio output channel out of range, clamping to max\n";
        #ifdef NDEBUG
        std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
        #endif 
        assert(false);
        channel = maxChannels-1;
    }
    
    return processor.channels[channel];
    
}

pdsp::Patchable& pdsp::Engine::audio_in( int channel ){
    
    if(channel < 0 ) channel = 0;
    int maxChannels = inputs.size();
  
    if(channel >= maxChannels){
        std::cout<<"[pdsp] warning! requested audio input channel out of range, clamping to max\n";
        #ifdef NDEBUG
        std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
        #endif 
        assert(false);
        channel = maxChannels-1;
    }
    
    return inputs[channel].out_signal();
    
}

pdsp::Patchable& pdsp::Engine::blackhole( ) {
    return processor.blackhole;
}

void pdsp::Engine::setChannels( int inputChannels, int outputChannels ){
    if(this->inputChannels != inputChannels){
        this->inputChannels = inputChannels;
        inputs.resize(inputChannels);   
    }
    
    if(this->outputChannels != outputChannels){
        this->outputChannels = outputChannels;
        processor.resize(outputChannels);
    }

}

void pdsp::Engine::setup( int sampleRate, int bufferSize, int nBuffers){
 
    ofLogNotice()<<"[pdsp] engine: starting with parameters: buffer size = "<<bufferSize<<" | sample rate = "<<sampleRate<<" | "<<inputChannels<<" inputs | "<<outputChannels<<" outputs\n";
   
    if ( nBuffers < 1 ) nBuffers = 1;
    
    // close if engine is already running with another settings
    if(state!=closedState){ 
        ofLogNotice()<<"[pdsp] engine: changing setup, shutting down stream";
        stop();
        if( inStreamActive ){
            inputStream.close();
        }
        if( outStreamActive ){
            outputStream.close();
        }
        
        #ifdef TARGET_OF_IOS
        ofSoundStreamClose();
        #endif  
        
        ofLogNotice()<<"[pdsp] engine: changing setup, releasing resources...";
        pdsp::releaseAll();
        
        state = closedState;
        ofSleepMillis( 20 );
        ofLogNotice()<<"...done";
    }
    
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

    #ifndef TARGET_OF_IOS
    ofLogNotice()<<"[pdsp] engine: started | buffer size = "<<outputStream.getBufferSize()<<" | sample rate = "<<outputStream.getSampleRate()<<" | "<<outputStream.getNumInputChannels()<<" inputs | "<<outputStream.getNumOutputChannels()<<" outputs\n";
    #endif
    
}

void pdsp::Engine::start(){
    if(inStreamActive && state < startedState){
        inputStream.start();
    }
    if(outStreamActive && state < startedState){
        outputStream.start();
    }
    #ifdef TARGET_OF_IOS
    ofSoundStreamStart();
    #endif    
    state = startedState;
}

void pdsp::Engine::stop(){
    if(state==startedState){
        if( inStreamActive ){
            inputStream.stop();
        }
        if( outStreamActive ){
            outputStream.stop();
        }    
        #ifdef TARGET_OF_IOS
        ofSoundStreamStop();	
        #endif    
        state = stoppedState;
    }
}

void pdsp::Engine::close(){
    
    std::cout<<"[pdsp] engine: closing...\n";
    
    stop();

#ifndef __ANDROID__
    if(hasMidiIn){
        for( pdsp::midi::Input * &in : midiIns ){
            in->closePort();
        } 
    }
#endif

    if(hasOscIn){
        for( pdsp::osc::Input * &in : oscIns){
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
    
    #ifdef TARGET_OF_IOS
    ofSoundStreamClose();
    #endif  
    pdsp::releaseAll();
    
    state = closedState;
    std::cout<<"[pdsp] engine: audio streams closed and resources released\n";    

}

void pdsp::Engine::audioOut(ofSoundBuffer &outBuffer) {
    
    int bufferSize =  outBuffer.getNumFrames() ;
    // score and playhead processing
    score.process( bufferSize );

#ifndef __ANDROID__
    // midi input processing
    if(hasMidiIn){
        for( pdsp::midi::Input * &in : midiIns){
            in->processMidi( bufferSize );
        } 
        for(int i=0; i<(int)controllers.size(); ++i){
            controllers[i]->processMidi( *(controllerLinkedMidis[i]), bufferSize );
        }
    }
#endif

    if(hasOscIn){
        for( pdsp::osc::Input * &in : oscIns){
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

void pdsp::Engine::audioIn (ofSoundBuffer &inBuffer) {
    for( int i=0; i<inputChannels; i++){
        inputs[i].copyInterleavedInput( inBuffer.getBuffer().data(), i,  inBuffer.getNumChannels(), inBuffer.getNumFrames());
    }
}

#ifndef __ANDROID__
void pdsp::Engine::addMidiController( pdsp::Controller & controller, pdsp::midi::Input & midiIn ){
    
    bool midiInFound = false;
    for( pdsp::midi::Input * &ptr : midiIns ){
        if( ptr == &midiIn ){
            midiInFound = true;
        } 
    }
    if( ! midiInFound ){
        midiIns.push_back( &midiIn );
    }
    
    bool midiControllerFound = false;
    
    for( pdsp::Controller * &ptr : controllers ){
        if( ptr == &controller ){
            midiControllerFound = true;
            std::cout<<"[pdsp] warning! you have already added this controller, you shouldn't add it twice\n";
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

void  pdsp::Engine::addMidiOut( pdsp::midi::Output & midiOut ){
    addExternalOut( midiOut );
}
#endif

#ifndef TARGET_OF_IOS
#ifndef __ANDROID__
void pdsp::Engine::addSerialOut( pdsp::serial::Output & serialOut ) {
    addExternalOut( serialOut ); 
}
#endif
#endif

void pdsp::Engine::addExternalOut( pdsp::ExtSequencer & externalOut ) {
   
    bool externalOutFound = false;
    for( pdsp::ExtSequencer * &ptr : externalOuts ){
        if( ptr == &externalOut ){
            externalOutFound = true;
            std::cout<<"[pdsp] warning! you have already added this external output to the engine, you shouldn't add it twice\n";
            pdsp::pdsp_trace();
        } 
    }
    if( ! externalOutFound ){
        externalOuts.push_back( &externalOut );
    }
    hasExternalOut = true;
}

void pdsp::Engine::addOscInput( pdsp::osc::Input & oscInput ) {
    bool oscInputFound = false;
    for( pdsp::osc::Input * &ptr : oscIns ){
        if( ptr == &oscInput ){
            oscInputFound = true;
            std::cout<<"[pdsp] warning! you have already added this OSC input to the engine, you shouldn't add it twice\n";
            pdsp::pdsp_trace();
        } 
    }
    if( ! oscInputFound ){
        oscIns.push_back( &oscInput );
    }
    hasOscIn = true;    
}

void pdsp::Engine::test( bool testingActive, float testingDB ){
    if( testingActive ){
        DBtoLin::eval(testingDB) >> testAmp.in_mod();
    }else{
        0.0f >> testAmp.in_mod();
    }
}

pdsp::Patchable & pdsp::Engine::out_bar_ms(){
    return barTime.out_signal();
}

void pdsp::Engine::setBackgroundAudio( bool active ){
    bBackgroundAudio = active;
}
