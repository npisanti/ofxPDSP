
#include "ofxPDSPMidiIn.h"

#ifndef __ANDROID__

#define OFXPDSP_MIDIKEYSPROCESSOR_MESSAGERESERVE 128

ofxPDSPMidiIn::ofxPDSPMidiIn(){
    midiMessageVectorA.reserve(OFXPDSP_MIDIKEYSPROCESSOR_MESSAGERESERVE);
    midiMessageVectorA.clear();
    midiMessageVectorB.reserve(OFXPDSP_MIDIKEYSPROCESSOR_MESSAGERESERVE);
    midiMessageVectorA.clear();
    readVector = &midiMessageVectorA;
    writeVector = &midiMessageVectorB;
    
    midiIn_p = nullptr;
    connected = false;
    
}

ofxPDSPMidiIn::~ofxPDSPMidiIn(){
    closePort();
}

void ofxPDSPMidiIn::linkToMidiIn(ofxMidiIn &midiInput){
    if(connected){
        closePort();
    }
    
    midiIn_p = &midiInput;
    midiIn_p->addListener(this); // add ofApp as a listener
    connected = true;
}


void ofxPDSPMidiIn::openPort(int index){
    if(connected){
        closePort();
    }
    
    midiIn.openPort(index); 
    
    if(midiIn.isOpen()){
        midiIn_p = &midiIn;
        midiIn_p->addListener(this); // add ofApp as a listener
        connected = true;
        bufferChrono = std::chrono::high_resolution_clock::now();
    }
}

void ofxPDSPMidiIn::closePort(){
    if(connected){
        connected = false; 
        midiIn_p->removeListener(this);
        if(midiIn.isOpen()){
            midiIn.closePort();
        }       
        midiIn_p = nullptr;
    }
}

void ofxPDSPMidiIn::listPorts(){
    midiIn.listPorts(); // print input ports to console
}


void ofxPDSPMidiIn::prepareToPlay( int expectedBufferSize, double sampleRate ){
    oneSlashMicrosecForSample = 1.0 / (1000000.0 / sampleRate);
}

void ofxPDSPMidiIn::releaseResources(){}


const std::vector<_ofxPositionedMidiMessage> & ofxPDSPMidiIn::getMessageVector() const{
    return *readVector;
}


void ofxPDSPMidiIn::newMidiMessage(ofxMidiMessage& eventArgs) noexcept{
    	
    midiMutex.lock();
        // calculate the right offset inside the bufferSize
        std::chrono::duration<double> offset = std::chrono::high_resolution_clock::now() - bufferChrono; 
        int sampleOffset =  static_cast<int>(  
            static_cast <double>( std::chrono::duration_cast<std::chrono::microseconds>(offset).count()) * oneSlashMicrosecForSample);
        writeVector->push_back(_ofxPositionedMidiMessage(eventArgs, sampleOffset));
    midiMutex.unlock();
    
}

void ofxPDSPMidiIn::processMidi( const int &bufferSize ) noexcept{
    if(connected){
        midiMutex.lock();
            //switch buffers
            std::vector<_ofxPositionedMidiMessage>* temp = readVector;
            readVector = writeVector;
            writeVector = temp;
            //update chrono
            bufferChrono = std::chrono::high_resolution_clock::now();
            //clear buffer to write
            writeVector->clear(); 
        midiMutex.unlock();
        
        //now sanitize messages to bufferSize
        for(_ofxPositionedMidiMessage &msg : *readVector){
            if(msg.sample >= bufferSize){ msg.sample = bufferSize-1; } else 
            if(msg.sample < 0 ) { msg.sample = 0; }
        }            
    }
}

#endif