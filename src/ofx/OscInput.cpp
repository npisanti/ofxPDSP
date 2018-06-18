
#include "OscInput.h"

#define OFXPDSP_OSCINPUT_MESSAGERESERVE 128

pdsp::osc::Input::OscChannel::OscChannel(){
    
    key = "";
    //mode = Gate;
    messageBuffer = nullptr;
    gate_out = nullptr;
    value_out = nullptr;
    
}

void pdsp::osc::Input::OscChannel::deallocate(){
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


pdsp::osc::Input::Input() {
    
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
    daemonRefreshRate = 500;
}   


pdsp::osc::Input::~Input(){
    if(connected){
        close();
    }
    
    for (size_t i = 0; i < oscChannels.size(); ++i){
        oscChannels[i]->deallocate();
        delete oscChannels[i];
        oscChannels[i] = nullptr;
    }
}

void pdsp::osc::Input::setVerbose( bool verbose ){
    this->verbose = verbose;
}


void pdsp::osc::Input::openPort( int port ) {
    if(connected){
        close();
    }
    
    receiver.setup( port );
    
    startDaemon();
    
    connected = true;
    bufferChrono = chrono::high_resolution_clock::now();
}



void pdsp::osc::Input::close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down OSC out\n";
        //stop the daemon before
        closeDaemon();

        connected = false;        
    }
}



pdsp::SequencerGateOutput& pdsp::osc::Input::out_trig( string oscAddress ) {
    
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
    osc->gate_out = new pdsp::SequencerGateOutput();
    osc->gate_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->gate_out);

}


pdsp::SequencerValueOutput& pdsp::osc::Input::out_value( string oscAddress ) {
   
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
    osc->value_out = new pdsp::SequencerValueOutput();
    osc->value_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->value_out);    

}

void pdsp::osc::Input::clearAll(){
    sendClearMessages = true;
}

void pdsp::osc::Input::prepareToPlay( int expectedBufferSize, double sampleRate ){
    oneSlashMicrosecForSample = 1.0 / (1000000.0 / sampleRate);
}

void pdsp::osc::Input::releaseResources(){}



void pdsp::osc::Input::daemonFunction() noexcept{
    
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

        this_thread::sleep_for(std::chrono::microseconds(daemonRefreshRate));

    }
   
    if(verbose) cout<<"[pdsp] closing OSC input daemon thread\n";
}
    

void pdsp::osc::Input::processOsc( int bufferSize ) noexcept {
    
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


void pdsp::osc::Input::startDaemon(){ // OK
    if(verbose) cout<<"[pdsp] starting OSC input daemon\n";
    runDaemon = true;
    daemonThread = thread( daemonFunctionWrapper, this );   
    
}


void pdsp::osc::Input::closeDaemon(){
 
    runDaemon = false;
    daemonThread.detach();

}
        
void pdsp::osc::Input::daemonFunctionWrapper(pdsp::osc::Input* parent){
    parent->daemonFunction();
}
    
    

