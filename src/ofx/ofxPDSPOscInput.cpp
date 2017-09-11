
#include "ofxPDSPOscInput.h"


#define OFXPDSP_OSCINPUT_MESSAGERESERVE 128

ofxPDSPOscInput::OscChannel::OscChannel(){
    
    key = "";
    //mode = Gate;
    messageBuffer = nullptr;
    gate_out = nullptr;
    value_out = nullptr;
    
}

void ofxPDSPOscInput::OscChannel::deallocate(){
    if( messageBuffer != nullptr ){
        delete messageBuffer;
    }
    if( gate_out != nullptr ) {
        delete gate_out;
    }
    if ( value_out != nullptr ) {
        delete value_out;
    }            
}


ofxPDSPOscInput::ofxPDSPOscInput() {
    
    //oscChannels.reserve(64);
    oscChannels.clear();

    oscMessageVectorA.reserve(OFXPDSP_OSCINPUT_MESSAGERESERVE);
    oscMessageVectorA.clear();
    oscMessageVectorB.reserve(OFXPDSP_OSCINPUT_MESSAGERESERVE);
    oscMessageVectorA.clear();
    readVector = &oscMessageVectorA;
    writeVector = &oscMessageVectorB;

    connected = false;
    
    runDaemon = false;
    daemonRefreshRate = 1;
}   


ofxPDSPOscInput::~ofxPDSPOscInput(){
    if(connected){
        close();
    }
    
    for (size_t i = 0; i < oscChannels.size(); ++i){
        oscChannels[i]->deallocate();
        delete oscChannels[i];
        oscChannels[i] = nullptr;
    }
}

void ofxPDSPOscInput::setVerbose( bool verbose ){
    this->verbose = verbose;
}


void ofxPDSPOscInput::openPort( int port ) {
    if(connected){
        close();
    }
    
    receiver.setup( port );
    
    startDaemon();
    
    connected = true;
    bufferChrono = chrono::high_resolution_clock::now();
}



void ofxPDSPOscInput::close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down OSC out\n";
        //stop the daemon before
        closeDaemon();

        connected = false;        
    }
}



pdsp::GateSequencer& ofxPDSPOscInput::out_trig( string oscAddress ) {
    
    for ( OscChannel* & osc : oscChannels ){
        if( osc->key == oscAddress ) {
            if( osc->gate_out != nullptr ){
                return *(osc->gate_out);
            }else{
                cout<<"[pdsp] warning! this osc string was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidGate;
            }
        }
    }
    
    // not found
    OscChannel* osc = new OscChannel();
    osc->key = oscAddress;
    //osc->mode = Gate;
    osc->messageBuffer = new pdsp::MessageBuffer();
    osc->gate_out = new pdsp::GateSequencer();
    osc->gate_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->gate_out);

}


pdsp::ValueSequencer& ofxPDSPOscInput::out_value( string oscAddress ) {
   
    for ( OscChannel* & osc : oscChannels ){
        if( osc->key == oscAddress ) {
            if( osc->value_out != nullptr ){
                return *(osc->value_out);
            }else{
                cout<<"[pdsp] warning! this osc string was already used as gate output, returning dummy value output\n";
                pdsp::pdsp_trace();
                return invalidValue;
            }
        }
    }

    // not found
    OscChannel* osc = new OscChannel();
    osc->key = oscAddress;
    //osc->mode = Value;
    osc->messageBuffer = new pdsp::MessageBuffer();
    osc->value_out = new pdsp::ValueSequencer();
    osc->value_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->value_out);    

}

void ofxPDSPOscInput::clearAll(){
    sendClearMessages = true;
}

void ofxPDSPOscInput::prepareToPlay( int expectedBufferSize, double sampleRate ){
    oneSlashMicrosecForSample = 1.0 / (1000000.0 / sampleRate);
}

void ofxPDSPOscInput::releaseResources(){}



void ofxPDSPOscInput::daemonFunction() noexcept{
    
    while (runDaemon){
        
        while(receiver.hasWaitingMessages()){
            
            ofxOscMessage osc;
            receiver.getNextMessage(osc);
            
            oscMutex.lock();
                // calculate the right offset inside the bufferSize
                chrono::duration<double> offset = chrono::high_resolution_clock::now() - bufferChrono; 
                int sampleOffset =  static_cast<int>(  
                    static_cast <double>( chrono::duration_cast<chrono::microseconds>(offset).count()) * oneSlashMicrosecForSample);
                writeVector->push_back(_ofxPositionedOscMessage( osc, sampleOffset));
            oscMutex.unlock();
        }

        this_thread::sleep_for(std::chrono::milliseconds(daemonRefreshRate));

    }
   
    if(verbose) cout<<"[pdsp] closing OSC input daemon thread\n";
}
    

void ofxPDSPOscInput::processOsc( int bufferSize ) noexcept {
    
    if(connected){
        
        oscMutex.lock();
            //switch buffers
            vector<_ofxPositionedOscMessage>* temp = readVector;
            readVector = writeVector;
            writeVector = temp;
            //update chrono
            bufferChrono = chrono::high_resolution_clock::now();
            //clear buffer to write
            writeVector->clear(); 
        oscMutex.unlock();
        
        //now sanitize messages to bufferSize
        for(_ofxPositionedOscMessage &msg : *readVector){
            if(msg.sample >= bufferSize){ msg.sample = bufferSize-1; } else
            if(msg.sample < 0 ) { msg.sample = 0; }
        }
        
        // clean the message buffers
        for (size_t i = 0; i < oscChannels.size(); ++i){
            oscChannels[i]->messageBuffer->clearMessages();

            if(sendClearMessages){
                oscChannels[i]->messageBuffer->addMessage(0.0f, 0);
            }
        }
        
        // adds the messages to the buffers, only the first arg of each osc message is read, as float
        for(_ofxPositionedOscMessage &osc : *readVector){
            for (size_t i = 0; i < oscChannels.size(); ++i){
                if(osc.message.getAddress() == oscChannels[i]->key){
                    oscChannels[i]->messageBuffer->addMessage( osc.message.getArgAsFloat(0), osc.sample );
                }
            }
        }
        
        // now process all the linked sequencers
        for (size_t i = 0; i < oscChannels.size(); ++i){
            oscChannels[i]->messageBuffer->processDestination(bufferSize);
        }
        
    }
    
}


void ofxPDSPOscInput::startDaemon(){ // OK
    if(verbose) cout<<"[pdsp] starting OSC input daemon\n";
    runDaemon = true;
    daemonThread = thread( daemonFunctionWrapper, this );   
    
}


void ofxPDSPOscInput::closeDaemon(){
 
    runDaemon = false;
    daemonThread.detach();

}
        
void ofxPDSPOscInput::daemonFunctionWrapper(ofxPDSPOscInput* parent){
    parent->daemonFunction();
}
    
    

