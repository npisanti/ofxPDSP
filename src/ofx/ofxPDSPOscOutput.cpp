
#include "ofxPDSPOscOutput.h"

#define OFXPDSP_OSCOUTPUTCIRCULARBUFFERSIZE 1024


ofxPDSPOscOutput::ScheduledOscMessage::ScheduledOscMessage(){  };

ofxPDSPOscOutput::ScheduledOscMessage::ScheduledOscMessage(ofxOscMessage message,  chrono::high_resolution_clock::time_point schedule) {

    this->message = message;
    this->scheduledTime = schedule;
                    
};

ofxPDSPOscOutput::ScheduledOscMessage::ScheduledOscMessage(const ofxPDSPOscOutput::ScheduledOscMessage &other){
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
}

ofxPDSPOscOutput::ScheduledOscMessage& ofxPDSPOscOutput::ScheduledOscMessage::operator= (const ofxPDSPOscOutput::ScheduledOscMessage &other){
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
    return *this;
}

ofxPDSPOscOutput::ScheduledOscMessage::~ScheduledOscMessage(){}


bool ofxPDSPOscOutput::scheduledSort(const ScheduledOscMessage &lhs, const ScheduledOscMessage &rhs ){
    return (lhs.scheduledTime < rhs.scheduledTime);
}


//-------------------------------------------------------------------------------------------

ofxPDSPOscOutput::ofxPDSPOscOutput(){
    inputs.reserve(128);
    messagesToSend.reserve(128);
    messagesToSend.clear();
    verbose = false;
    selectedAddress = "uninitialized";
    
    connected = false;
    
    chronoStarted = false;
    
    //midi daemon init
    messagesReady = false;
    runDaemon = false;
    
    //processing init

    circularMax = OFXPDSP_OSCOUTPUTCIRCULARBUFFERSIZE;
    circularBuffer.resize(circularMax);
    circularRead  = 0;
    circularWrite = 0;
    
    //testing
    messageCount = 0;
}

ofxPDSPOscOutput::~ofxPDSPOscOutput(){
    if(connected){
        close();
    }
}

void ofxPDSPOscOutput::setVerbose( bool verbose ){
    this->verbose = verbose;
}


void ofxPDSPOscOutput::openPort(const string &hostname, int port ) {
    if(connected){
        close();
    }
    
    sender.setup( hostname, port );
    
    startDaemon();
    
    connected = true;

}



void ofxPDSPOscOutput:: close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down OSC out\n";
        //stop the daemon before
        closeDaemon();

        connected = false;        
    }
}

pdsp::ExtSequencer& ofxPDSPOscOutput::address( string oscAddress ) {
    selectedAddress = oscAddress;
    return *this;
}

void ofxPDSPOscOutput::linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
#ifndef NDEBUG
    cout<<"[pdsp] linking message buffer\n";
#endif
    inputs.push_back(&messageBuffer);
    addresses.push_back(selectedAddress);
}

void ofxPDSPOscOutput::unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    
    int i=0;
    for (vector<pdsp::MessageBuffer*>::iterator it = inputs.begin(); it != inputs.end(); ++it){
        if (*it == &messageBuffer){
            inputs.erase(it);
            vector<string>::iterator linkedAddress = addresses.begin() + i;
            addresses.erase(linkedAddress);
            return;
        }
        i++;
    }
    
}

void ofxPDSPOscOutput::prepareToPlay( int expectedBufferSize, double sampleRate ){
    usecPerSample = 1000000.0 / sampleRate;
    
}

void ofxPDSPOscOutput::releaseResources() {}

// OK -----------------------------------------------^^^^^^^^^^^-------------

void ofxPDSPOscOutput::process( int bufferSize ) noexcept{
    
    if(connected){
        //clear messages
        messagesToSend.clear();
        
        //add note messages
        int maxBuffer = inputs.size();
        
        if(chronoStarted){
            chrono::nanoseconds bufferOffset = chrono::nanoseconds (static_cast<long> ( bufferSize * usecPerSample ));
            bufferChrono = bufferChrono + bufferOffset;
        }else{
            bufferChrono = chrono::high_resolution_clock::now();
            chronoStarted = true;
        }
        
        for( int i=0; i<maxBuffer; ++i ){
            
            pdsp::MessageBuffer* messageBuffer = inputs[i];
            //string msg_address = addresses[i];
            
            /* // OLD WAY
            bufferChrono = chrono::high_resolution_clock::now();
            */
            
            int bufferMax = messageBuffer->size();

            for(int n=0; n<bufferMax; ++n){                
                //format message to sent
                float msg_value = messageBuffer->messages[n].value;
                int msg_sample = messageBuffer->messages[n].sample;
                
                chrono::nanoseconds offset = chrono::nanoseconds (static_cast<long> ( msg_sample * usecPerSample ));
                chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                ofxOscMessage osc;
                osc.setAddress( addresses[i] );
                osc.addFloatArg( msg_value );
                
                messagesToSend.push_back( ScheduledOscMessage(osc, scheduleTime) );
            }
        }
        
        //sort messages to send
        sort(messagesToSend.begin(), messagesToSend.end(), scheduledSort);
        
        //send to daemon
        if( ! messagesToSend.empty()){
            prepareForDaemonAndNotify();
        }
    }//end checking connected
}

void ofxPDSPOscOutput::startDaemon(){ // OK
    
    runDaemon = true;
    daemonThread = thread( daemonFunctionWrapper, this );   
    
}

void ofxPDSPOscOutput::prepareForDaemonAndNotify(){
    
    unique_lock<mutex> lck (outMutex);  
    //send messages in circular buffer
    for(ScheduledOscMessage &msg : messagesToSend){
        circularBuffer[circularWrite] = msg;
        ++circularWrite;
        if(circularWrite==circularMax){
            circularWrite = 0;
        }
    }
    messagesReady = true;
    outCondition.notify_all();
    
}
   
    
void ofxPDSPOscOutput::daemonFunctionWrapper(ofxPDSPOscOutput* parent){
    parent->daemonFunction();
}
   
    
void ofxPDSPOscOutput::daemonFunction() noexcept{
    
    while (runDaemon){

        //midiMutex.lock();
        unique_lock<mutex> lck (outMutex);
        while(!messagesReady) outCondition.wait(lck);
        
        if(circularRead != circularWrite){
            
            ScheduledOscMessage& nextMessage = circularBuffer[circularRead];
            
            if( nextMessage.scheduledTime < chrono::high_resolution_clock::now() ){ //we have to process the scheduled midi
                
                // SEND MESSAGES HERE
                sender.sendMessage( nextMessage.message, false );
                
                #ifndef NDEBUG
                    if(verbose) cout << "[pdsp] OSC message: address = "<< nextMessage.message.getAddress() << " | value = "<<(int)nextMessage.message.getArgAsFloat(0)<<"\n";
                #endif
                
                ++circularRead;
                if(circularRead == circularMax){
                    circularRead = 0;
                }
            }

        }else{
            messagesReady = false;
        }

        this_thread::yield();

    }
   
    if(verbose) cout<<"[pdsp] closing OSC out daemon thread\n";
}
    
 
    
void ofxPDSPOscOutput::closeDaemon(){
    runDaemon = false;
    
    unique_lock<mutex> lck (outMutex);  
    //set messages in circular buffer
    messagesReady = true;
    outCondition.notify_all();
    daemonThread.detach();

}
    
    
    
