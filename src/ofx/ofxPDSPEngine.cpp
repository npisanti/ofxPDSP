
#include "ofxPDSPEngine.h"


ofxPDSPEngine::ofxPDSPEngine(){
    inputID = 0;
    outputID = 0;
    inStreamActive = false;
    outStreamActive = false;
    
    inputChannels = outputChannels = -1; // for the next method to work
    setChannels(0, 2);
    
    state = closedState;

    midiIns.reserve(10);
    controllers.reserve(20);
    controllerLinkedMidis.reserve(20);
    
    midiOuts.reserve(10);
    
    midiIns.clear();
    controllers.clear();
    hasMidiIn = false;
    hasMidiOut = false;

    ofAddListener( ofEvents().exit, this, &ofxPDSPEngine::onExit );
}


ofxPDSPEngine::~ofxPDSPEngine(){
    if(state!= closedState){
        close();
    }
}

void ofxPDSPEngine::onExit( ofEventArgs &args){
    if(state!= closedState){
        close();
    }
}

void ofxPDSPEngine::listDevices(){
    ofSoundStreamListDevices();
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
   
    if ( nBuffers < 2 ) nBuffers = 2;
    
    // prepare all the units / modules
    pdsp::prepareAllToPlay(bufferSize, static_cast<double>(sampleRate) );
    
    // starts engine
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

    state = startedState;
    cout<<"[pdsp] engine: started | buffer size = "<<bufferSize<<" | sample rate = "<<sampleRate
        <<" | "<<inputChannels<<" inputs | "<<outputChannels<<" outputs\n";
}

void ofxPDSPEngine::start(){
    if(inStreamActive && state < startedState){
        inputStream.start();
    }
    if(outStreamActive && state < startedState){
        outputStream.start();
    }
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
    }
}

void ofxPDSPEngine::close(){
    
    cout<<"[pdsp] engine: closing...\n";
    
    stop();    
    
    if(hasMidiIn){
        for( ofxPDSPMidiIn * &in : midiIns ){
            in->closePort();
        } 
    }

    if(hasMidiOut){
        for( ofxPDSPMidiOut * &out : midiOuts ){
            out->closePort();
        } 
    }
    
    if( inStreamActive ){
        inputStream.close();
    }
    if( outStreamActive ){
        outputStream.close();
    }
    
    pdsp::releaseAll();
    
    state = closedState;
    cout<<"[pdsp] engine: audio streams closed and resources released\n";    

}

void ofxPDSPEngine::audioOut(ofSoundBuffer &outBuffer) {
    
    // score and playhead processing
    score.process( outBuffer.getNumFrames() );
    
    // midi input processing
    if(hasMidiIn){
        for( ofxPDSPMidiIn * &in : midiIns){
            in->processMidi( outBuffer.getNumFrames() );
        } 
        for(int i=0; i<(int)controllers.size(); ++i){
            controllers[i]->processMidi( *(controllerLinkedMidis[i]), outBuffer.getNumFrames() );
        }
    }
    
    // midi output processing
    if(hasMidiOut){
        for( ofxPDSPMidiOut * &out : midiOuts){
            out->process();
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
            #ifdef NDEBUG
            std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
            #endif 
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
   
    bool midiOutFound = false;
    for( ofxPDSPMidiOut * &ptr : midiOuts ){
        if( ptr == &midiOut ){
            midiOutFound = true;
            cout<<"[pdsp] warning! you have already added this midi out to the engine, you shouldn't add it twice\n";
            #ifdef NDEBUG
            std::cout<<"[pdsp] build in debug mode for triggering an assert\n";
            #endif 
            assert(false);
        } 
    }
    if( ! midiOutFound ){
        midiOuts.push_back( &midiOut );
    }
    hasMidiOut = true;
}


