
#include "MidiIn.h"

#ifndef __ANDROID__

#define OFXPDSP_MIDIREADVECTOR_MESSAGERESERVE 128
#define OFXPDSP_MIDICIRCULARBUFFER_SIZE 4096

pdsp::midi::Input::Input(){

    midiIn_p = nullptr;
    connected = false;
    readVector.reserve(OFXPDSP_MIDIREADVECTOR_MESSAGERESERVE);
    circularBuffer.resize( OFXPDSP_MIDICIRCULARBUFFER_SIZE );
    
    lastread = 0;
    index = 0;
}

pdsp::midi::Input::~Input(){
    closePort();
}

void pdsp::midi::Input::linkToMidiIn(ofxMidiIn &midiInput){
    if(connected){
        closePort();
    }
    
    midiIn_p = &midiInput;
    midiIn_p->addListener(this); // add ofApp as a listener
    connected = true;
}

void pdsp::midi::Input::initPort() {
    if(midiIn.isOpen()){
        midiIn_p = &midiIn;
        midiIn_p->addListener(this); // add ofApp as a listener
        connected = true;
        bufferChrono = std::chrono::high_resolution_clock::now();
    }
}

void pdsp::midi::Input::openPort(int index){
    if(connected){ closePort(); }
    midiIn.openPort(index); 
    initPort();
}

void pdsp::midi::Input::openPort( std::string name){
    if(connected){ closePort(); }
    midiIn.openPort(name); 
    initPort();
}

void pdsp::midi::Input::openVirtualPort( std::string name){
    if(connected){ closePort(); }
    midiIn.openVirtualPort( name ); 
    initPort();
}

void pdsp::midi::Input::closePort(){
    if(connected){
        connected = false; 
        midiIn_p->removeListener(this);
        if(midiIn.isOpen()){
            midiIn.closePort();
        }       
        midiIn_p = nullptr;
    }
}

void pdsp::midi::Input::listPorts(){
    midiIn.listInPorts(); // print input ports to console
}


void pdsp::midi::Input::prepareToPlay( int expectedBufferSize, double sampleRate ){
    oneSlashMicrosecForSample = 1.0 / (1000000.0 / sampleRate);
}

void pdsp::midi::Input::releaseResources(){}


const std::vector<pdsp::_PositionedMidiMessage> & pdsp::midi::Input::getMessageVector() const{
    return readVector;
}


void pdsp::midi::Input::newMidiMessage(ofxMidiMessage& eventArgs) noexcept{
    	
    // calculate the right offset inside the bufferSize
    int write = index +1;
    if(write>=(int)circularBuffer.size()){ write = 0; } 
    
    circularBuffer[write].message = eventArgs;
    circularBuffer[write].timepoint = std::chrono::high_resolution_clock::now();
    
    index = write;    
}

void pdsp::midi::Input::pushToReadVector( pdsp::_PositionedMidiMessage & message ){
        std::chrono::duration<double> offset = message.timepoint - bufferChrono; 
        message.sample = static_cast<int>( static_cast <double>( std::chrono::duration_cast<std::chrono::microseconds>(offset).count()) * oneSlashMicrosecForSample);
        readVector.push_back( message );    
}

void pdsp::midi::Input::processMidi( const int &bufferSize ) noexcept{
    if(connected){
        
        readVector.clear();
        int read = index;

        if(read<lastread){ // two segments
            for(int i=lastread+1; i<(int)circularBuffer.size(); ++i){
                pushToReadVector( circularBuffer[i] );
            }            
            for( int i=0; i<=read; ++i){
                pushToReadVector( circularBuffer[i] );
            }            
        }else{
            for(int i=lastread+1; i<=read; ++i){
                pushToReadVector( circularBuffer[i] );
            }
        }
        
        lastread = read;
        bufferChrono = std::chrono::high_resolution_clock::now();
        
        //now sanitize messages to bufferSize
        for(_PositionedMidiMessage &msg : readVector){
            if(msg.sample >= bufferSize){ msg.sample = bufferSize-1; } else 
            if(msg.sample < 0 ) { msg.sample = 0; }
        }       
    }
}

#endif
