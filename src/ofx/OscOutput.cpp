
#include "OscOutput.h"

#define OFXPDSP_OSCOUTPUTCIRCULARBUFFERSIZE 1024


pdsp::osc::Output::ScheduledOscMessage::ScheduledOscMessage(){ 
    ofxOscMessage osc;
    osc.setAddress( "/unknown" );
    osc.addFloatArg( 0.0f );
    this->message = osc;
    this->scheduledTime = std::chrono::time_point<std::chrono::high_resolution_clock>::min();
 };

pdsp::osc::Output::ScheduledOscMessage::ScheduledOscMessage(ofxOscMessage message,  std::chrono::high_resolution_clock::time_point schedule) {
    this->message = message;
    this->scheduledTime = schedule;
};

pdsp::osc::Output::ScheduledOscMessage::ScheduledOscMessage(const pdsp::osc::Output::ScheduledOscMessage &other){
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
}

pdsp::osc::Output::ScheduledOscMessage& pdsp::osc::Output::ScheduledOscMessage::operator= (const pdsp::osc::Output::ScheduledOscMessage &other){
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
    return *this;
}

pdsp::osc::Output::ScheduledOscMessage::~ScheduledOscMessage(){}


bool pdsp::osc::Output::scheduledSort(const ScheduledOscMessage &lhs, const ScheduledOscMessage &rhs ){
    return (lhs.scheduledTime < rhs.scheduledTime);
}


//-------------------------------------------------------------------------------------------

pdsp::osc::Output::Output(){
    inputs.reserve(128);
    messagesToSend.reserve(128);
    messagesToSend.clear();
    verbose = false;
    selectedAddress = "uninitialized";
    
    connected = false;
    
    chronoStarted = false;
    
    //midi daemon init
    runDaemon = false;
    
    //processing init
    circularBuffer.resize(OFXPDSP_OSCOUTPUTCIRCULARBUFFERSIZE);
    writeindex = 0;
    send = 0;
    
    //testing
    messageCount = 0;
}

pdsp::osc::Output::~Output(){
    if(connected){
        close();
    }
}

void pdsp::osc::Output::setVerbose( bool verbose ){
    this->verbose = verbose;
}


void pdsp::osc::Output::openPort(const std::string &hostname, int port ) {
    if(connected){
        close();
    }
    
    sender.setup( hostname, port );
    
    startDaemon();
    
    connected = true;

}



void pdsp::osc::Output:: close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down OSC out\n";
        //stop the daemon before
        closeDaemon();

        connected = false;        
    }
}

pdsp::ExtSequencer& pdsp::osc::Output::address( std::string oscAddress ) {
    selectedAddress = oscAddress;
    return *this;
}

void pdsp::osc::Output::linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
#ifndef NDEBUG
    cout<<"[pdsp] linking message buffer\n";
#endif
    inputs.push_back(&messageBuffer);
    addresses.push_back(selectedAddress);
}

void pdsp::osc::Output::unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    
    int i=0;
    for (std::vector<pdsp::MessageBuffer*>::iterator it = inputs.begin(); it != inputs.end(); ++it){
        if (*it == &messageBuffer){
            inputs.erase(it);
            std::vector<std::string>::iterator linkedAddress = addresses.begin() + i;
            addresses.erase(linkedAddress);
            return;
        }
        i++;
    }
    
}

void pdsp::osc::Output::prepareToPlay( int expectedBufferSize, double sampleRate ){
    usecPerSample = 1000000.0 / sampleRate;
}

void pdsp::osc::Output::releaseResources() {}

// OK -----------------------------------------------^^^^^^^^^^^-------------

void pdsp::osc::Output::process( int bufferSize ) noexcept{
    
    if(connected){
        //clear messages
        messagesToSend.clear();
        
        //add note messages
        if(chronoStarted){
			std::chrono::nanoseconds bufferOffset = std::chrono::nanoseconds (static_cast<long> ( bufferSize * usecPerSample ));
            bufferChrono = bufferChrono + bufferOffset;
        }else{
            bufferChrono = std::chrono::high_resolution_clock::now();
            chronoStarted = true;
        }
        
        for( int i=0; i<(int)inputs.size(); ++i ){
            
            pdsp::MessageBuffer* messageBuffer = inputs[i];
            //std::string msg_address = addresses[i];
            
            for(int n=0; n<(int)messageBuffer->size(); ++n){                
                //format message to sent
                float msg_value = messageBuffer->messages[n].value;
                int msg_sample = messageBuffer->messages[n].sample;
                
				std::chrono::nanoseconds offset = std::chrono::nanoseconds (static_cast<long> ( msg_sample * usecPerSample ));
				std::chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                ofxOscMessage osc;
                osc.setAddress( addresses[i] );
                osc.addFloatArg( msg_value );
            
                messagesToSend.push_back( ScheduledOscMessage(osc, scheduleTime) );
            }
        }
        
        //sort messages to send
        sort(messagesToSend.begin(), messagesToSend.end(), scheduledSort);

        for(ScheduledOscMessage &msg : messagesToSend){
            int write = writeindex+1;
            if( write >= (int)circularBuffer.size() ){ write = 0; };
            circularBuffer[write] = msg;
            writeindex = write;
        }
        
    }//end checking connected
}

void pdsp::osc::Output::startDaemon(){ // OK
    
    runDaemon = true;
    daemonThread = std::thread( daemonFunctionWrapper, this );   
    
}
   
    
void pdsp::osc::Output::daemonFunctionWrapper(pdsp::osc::Output* parent){
    parent->daemonFunction();
}
   
    
void pdsp::osc::Output::daemonFunction() noexcept{
    
    while (runDaemon){

        while( send!=writeindex && circularBuffer[send].scheduledTime < std::chrono::high_resolution_clock::now() ){
            
            #ifndef NDEBUG
                if(verbose) cout << "[pdsp] OSC message: address = "<< circularBuffer[send].message.getAddress() << " | value = "<<(int)circularBuffer[send].message.getArgAsFloat(0)<<"\n";
            #endif
                       
            // SEND MESSAGES HERE
            sender.sendMessage( circularBuffer[send].message, false );

            send++;
            if( send >= int(circularBuffer.size()) ){
                send = 0;
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(500));

    }
   
    if(verbose) cout<<"[pdsp] closing OSC out daemon thread\n";
}
    
 
    
void pdsp::osc::Output::closeDaemon(){
    runDaemon = false;
    daemonThread.detach();
}
